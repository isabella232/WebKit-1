#!/usr/bin/env python
# Copyright (C) 2012 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


from webkitpy.layout_tests.models import test_expectations

from webkitpy.common.net import resultsjsonparser


TestExpectations = test_expectations.TestExpectations
TestExpectationParser = test_expectations.TestExpectationParser


class BuildBotPrinter(object):
    # This output is parsed by buildbots and must only be changed in coordination with buildbot scripts (see webkit.org's
    # Tools/BuildSlaveSupport/build.webkit.org-config/master.cfg: RunWebKitTests._parseNewRunWebKitTestsOutput
    # and chromium.org's buildbot/master.chromium/scripts/master/log_parser/webkit_test_command.py).

    def __init__(self, stream, debug_logging):
        self.stream = stream
        self.debug_logging = debug_logging

    def print_results(self, run_details):
        if self.debug_logging:
            self.print_result_summary(run_details.result_summary)
        self.print_unexpected_results(run_details.summarized_results)

    def _print(self, msg):
        self.stream.write(msg + '\n')

    def print_result_summary(self, result_summary):
        failed = result_summary.total_failures
        total = result_summary.total
        passed = total - failed - result_summary.remaining
        percent_passed = 0.0
        if total > 0:
            percent_passed = float(passed) * 100 / total

        self._print("=> Results: %d/%d tests passed (%.1f%%)" % (passed, total, percent_passed))
        self._print("")
        self._print_result_summary_entry(result_summary, test_expectations.NOW, "Tests to be fixed")

        self._print("")
        # FIXME: We should be skipping anything marked WONTFIX, so we shouldn't bother logging these stats.
        self._print_result_summary_entry(result_summary, test_expectations.WONTFIX,
            "Tests that will only be fixed if they crash (WONTFIX)")
        self._print("")

    def _print_result_summary_entry(self, result_summary, timeline, heading):
        total = len(result_summary.tests_by_timeline[timeline])
        not_passing = (total -
            len(result_summary.tests_by_expectation[test_expectations.PASS] &
                result_summary.tests_by_timeline[timeline]))
        self._print("=> %s (%d):" % (heading, not_passing))

        for result in TestExpectations.EXPECTATION_ORDER:
            if result in (test_expectations.PASS, test_expectations.SKIP):
                continue
            results = (result_summary.tests_by_expectation[result] &
                        result_summary.tests_by_timeline[timeline])
            desc = TestExpectations.EXPECTATION_DESCRIPTIONS[result]
            if not_passing and len(results):
                pct = len(results) * 100.0 / not_passing
                self._print("  %5d %-24s (%4.1f%%)" % (len(results), desc, pct))

    def print_unexpected_results(self, summarized_results):
        passes = {}
        flaky = {}
        regressions = {}

        def add_to_dict_of_lists(dict, key, value):
            dict.setdefault(key, []).append(value)

        def add_result(test, results, passes=passes, flaky=flaky, regressions=regressions):
            actual = results['actual'].split(" ")
            expected = results['expected'].split(" ")

            def is_expected(result):
                return (result in expected) or (result in ('TEXT', 'IMAGE+TEXT') and 'FAIL' in expected)

            if all(is_expected(actual_result) for actual_result in actual):
                # Don't print anything for tests that ran as expected.
                return

            if actual == ['PASS']:
                if 'CRASH' in expected:
                    add_to_dict_of_lists(passes, 'Expected to crash, but passed', test)
                elif 'TIMEOUT' in expected:
                    add_to_dict_of_lists(passes, 'Expected to timeout, but passed', test)
                else:
                    add_to_dict_of_lists(passes, 'Expected to fail, but passed', test)
            elif len(actual) > 1:
                # We group flaky tests by the first actual result we got.
                add_to_dict_of_lists(flaky, actual[0], test)
            else:
                add_to_dict_of_lists(regressions, results['actual'], test)

        resultsjsonparser.for_each_test(summarized_results['tests'], add_result)

        if len(passes) or len(flaky) or len(regressions):
            self._print("")
        if len(passes):
            for key, tests in passes.iteritems():
                self._print("%s: (%d)" % (key, len(tests)))
                tests.sort()
                for test in tests:
                    self._print("  %s" % test)
                self._print("")
            self._print("")

        if len(flaky):
            descriptions = TestExpectations.EXPECTATION_DESCRIPTIONS
            for key, tests in flaky.iteritems():
                result = TestExpectations.EXPECTATIONS[key.lower()]
                self._print("Unexpected flakiness: %s (%d)" % (descriptions[result], len(tests)))
                tests.sort()

                for test in tests:
                    result = resultsjsonparser.result_for_test(summarized_results['tests'], test)
                    actual = result['actual'].split(" ")
                    expected = result['expected'].split(" ")
                    result = TestExpectations.EXPECTATIONS[key.lower()]
                    # FIXME: clean this up once the old syntax is gone
                    new_expectations_list = [TestExpectationParser._inverted_expectation_tokens[exp] for exp in list(set(actual) | set(expected))]
                    self._print("  %s [ %s ]" % (test, " ".join(new_expectations_list)))
                self._print("")
            self._print("")

        if len(regressions):
            descriptions = TestExpectations.EXPECTATION_DESCRIPTIONS
            for key, tests in regressions.iteritems():
                result = TestExpectations.EXPECTATIONS[key.lower()]
                self._print("Regressions: Unexpected %s (%d)" % (descriptions[result], len(tests)))
                tests.sort()
                for test in tests:
                    self._print("  %s [ %s ]" % (test, TestExpectationParser._inverted_expectation_tokens[key]))
                self._print("")

        if len(summarized_results['tests']) and self.debug_logging:
            self._print("%s" % ("-" * 78))
