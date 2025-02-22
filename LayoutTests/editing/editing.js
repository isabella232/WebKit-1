//-------------------------------------------------------------------------------------------------------
// Java script library to run editing layout tests

var commandCount = 1;
var commandDelay = window.location.search.substring(1);
if (commandDelay == '')
    commandDelay = 0;
var selection = window.getSelection();

//-------------------------------------------------------------------------------------------------------

function execSetSelectionCommand(sn, so, en, eo) {
    window.getSelection().setBaseAndExtent(sn, so, en, eo);
}

// Args are startNode, startOffset, endNode, endOffset
function setSelectionCommand(sn, so, en, eo) {
    if (commandDelay > 0) {
        window.setTimeout(execSetSelectionCommand, commandCount * commandDelay, sn, so, en, eo);
        commandCount++;
    } else
        execSetSelectionCommand(sn, so, en, eo);
}

//-------------------------------------------------------------------------------------------------------

function execTransposeCharactersCommand() {
    document.execCommand("Transpose");
}
function transposeCharactersCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execTransposeCharactersCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execTransposeCharactersCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionLeftByCharacterCommand() {
    selection.modify("move", "left", "character");
}
function moveSelectionLeftByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionLeftByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionLeftByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionRightByCharacterCommand() {
    selection.modify("move", "Right", "character");
}
function moveSelectionRightByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionRightByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionRightByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionLeftByCharacterCommand() {
    selection.modify("extend", "left", "character");
}
function extendSelectionLeftByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionLeftByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionLeftByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionRightByCharacterCommand() {
    selection.modify("extend", "Right", "character");
}
function extendSelectionRightByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionRightByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionRightByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionForwardByCharacterCommand() {
    selection.modify("move", "forward", "character");
}
function moveSelectionForwardByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionForwardByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionForwardByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionForwardByCharacterCommand() {
    selection.modify("extend", "forward", "character");
}
function extendSelectionForwardByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionForwardByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionForwardByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionForwardByWordCommand() {
    selection.modify("move", "forward", "word");
}
function moveSelectionForwardByWordCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionForwardByWordCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionForwardByWordCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionForwardByWordCommand() {
    selection.modify("extend", "forward", "word");
}
function extendSelectionForwardByWordCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionForwardByWordCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionForwardByWordCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionForwardBySentenceCommand() {
    selection.modify("move", "forward", "sentence");
}
function moveSelectionForwardBySentenceCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionForwardBySentenceCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionForwardBySentenceCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionForwardBySentenceCommand() {
    selection.modify("extend", "forward", "sentence");
}
function extendSelectionForwardBySentenceCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionForwardBySentenceCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionForwardBySentenceCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionForwardByLineCommand() {
    selection.modify("move", "forward", "line");
}
function moveSelectionForwardByLineCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionForwardByLineCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionForwardByLineCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionForwardByLineCommand() {
    selection.modify("extend", "forward", "line");
}
function extendSelectionForwardByLineCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionForwardByLineCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionForwardByLineCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionForwardByLineBoundaryCommand() {
    selection.modify("move", "forward", "lineBoundary");
}
function moveSelectionForwardByLineBoundaryCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionForwardByLineBoundaryCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionForwardByLineBoundaryCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionForwardByLineBoundaryCommand() {
    selection.modify("extend", "forward", "lineBoundary");
}
function extendSelectionForwardByLineBoundaryCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionForwardByLineBoundaryCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionForwardByLineBoundaryCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionBackwardByCharacterCommand() {
    selection.modify("move", "backward", "character");
}
function moveSelectionBackwardByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionBackwardByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionBackwardByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionBackwardByCharacterCommand() {
    selection.modify("extend", "backward", "character");
}
function extendSelectionBackwardByCharacterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionBackwardByCharacterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionBackwardByCharacterCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionBackwardByWordCommand() {
    selection.modify("move", "backward", "word");
}
function moveSelectionBackwardByWordCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionBackwardByWordCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionBackwardByWordCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionBackwardByWordCommand() {
    selection.modify("extend", "backward", "word");
}
function extendSelectionBackwardByWordCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionBackwardByWordCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionBackwardByWordCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionBackwardBySentenceCommand() {
    selection.modify("move", "backward", "sentence");
}
function moveSelectionBackwardBySentenceCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionBackwardBySentenceCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionBackwardBySentenceCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionBackwardBySentenceCommand() {
    selection.modify("extend", "backward", "sentence");
}
function extendSelectionBackwardBySentenceCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionBackwardBySentenceCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionBackwardBySentenceCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionBackwardByLineCommand() {
    selection.modify("move", "backward", "line");
}
function moveSelectionBackwardByLineCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionBackwardByLineCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionBackwardByLineCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionBackwardByLineCommand() {
    selection.modify("extend", "backward", "line");
}
function extendSelectionBackwardByLineCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionBackwardByLineCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionBackwardByLineCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execExtendSelectionBackwardByLineBoundaryCommand() {
    selection.modify("extend", "backward", "lineBoundary");
}
function extendSelectionBackwardByLineBoundaryCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execExtendSelectionBackwardByLineBoundaryCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execExtendSelectionBackwardByLineBoundaryCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execMoveSelectionBackwardByLineBoundaryCommand() {
    selection.modify("move", "backward", "lineBoundary");
}
function moveSelectionBackwardByLineBoundaryCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execMoveSelectionBackwardByLineBoundaryCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execMoveSelectionBackwardByLineBoundaryCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function doubleClick(x, y) {
    eventSender.mouseMoveTo(x, y);
    eventSender.mouseDown();
    eventSender.mouseUp();
    eventSender.mouseDown();
    eventSender.mouseUp();
}

function doubleClickAtSelectionStart() {
    var rects = window.getSelection().getRangeAt(0).getClientRects();
    var x = rects[0].left;
    var y = rects[0].top;
    doubleClick(x, y);
}

//-------------------------------------------------------------------------------------------------------

function execBoldCommand() {
    document.execCommand("Bold");
    debugForDumpAsText("execBoldCommand");
}
function boldCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execBoldCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execBoldCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execUnderlineCommand() {
    document.execCommand("Underline");
    debugForDumpAsText("execUnderlineCommand");
}
function underlineCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execUnderlineCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execUnderlineCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execFontNameCommand() {
    document.execCommand("FontName", false, "Courier");
    debugForDumpAsText("execFontNameCommand");
}
function fontNameCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execFontNameCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execFontNameCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execFontSizeCommand(s) {
    if (arguments.length == 0 || s == undefined || s.length == 0)
        s = '12px';
    document.execCommand("FontSize", false, s);
    debugForDumpAsText("execFontSizeCommand");
}
function fontSizeCommand(s) {
    if (commandDelay > 0) {
        window.setTimeout(execFontSizeCommand, commandCount * commandDelay, s);
        commandCount++;
    }
    else {
        execFontSizeCommand(s);
    }
}

//-------------------------------------------------------------------------------------------------------

function execFontSizeDeltaCommand(s) {
    if (arguments.length == 0 || s == undefined || s.length == 0)
        s = '1px';
    document.execCommand("FontSizeDelta", false, s);
    debugForDumpAsText("execFontSizeDeltaCommand");
}
function fontSizeDeltaCommand(s) {
    if (commandDelay > 0) {
        window.setTimeout(execFontSizeDeltaCommand, commandCount * commandDelay, s);
        commandCount++;
    }
    else {
        execFontSizeDeltaCommand(s);
    }
}

//-------------------------------------------------------------------------------------------------------

function execItalicCommand() {
    document.execCommand("Italic");
    debugForDumpAsText("execItalicCommand");
}
function italicCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execItalicCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execItalicCommand();
    }
}


//-------------------------------------------------------------------------------------------------------

function execJustifyCenterCommand() {
    document.execCommand("JustifyCenter");
    debugForDumpAsText("execJustifyCenterCommand");
}
function justifyCenterCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execJustifyCenterCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execJustifyCenterCommand();
    }
}


//-------------------------------------------------------------------------------------------------------

function execJustifyLeftCommand() {
    document.execCommand("JustifyLeft");
    debugForDumpAsText("execJustifyLeftCommand");
}
function justifyLeftCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execJustifyLeftCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execJustifyLeftCommand();
    }
}


//-------------------------------------------------------------------------------------------------------

function execJustifyRightCommand() {
    document.execCommand("JustifyRight");
    debugForDumpAsText("execJustifyRightCommand");
}
function justifyRightCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execJustifyRightCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execJustifyRightCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execInsertHTMLCommand(html) {
    document.execCommand("InsertHTML", false, html);
    debugForDumpAsText("execInsertHTMLCommand");
}
function insertHTMLCommand(html) {
    if (commandDelay > 0) {
        window.setTimeout(execInsertHTMLCommand, commandCount * commandDelay, html);
        commandCount++;
    }
    else {
        execInsertHTMLCommand(html);
    }
}

//-------------------------------------------------------------------------------------------------------

function execInsertImageCommand(imgSrc) {
    document.execCommand("InsertImage", false, imgSrc);
    debugForDumpAsText("execInsertImageCommand");
}
function insertImageCommand(imgSrc) {
    if (commandDelay > 0) {
        window.setTimeout(execInsertImageCommand, commandCount * commandDelay, imgSrc);
        commandCount++;
    }
    else {
        execInsertImageCommand(imgSrc);
    }
}

//-------------------------------------------------------------------------------------------------------

function execInsertLineBreakCommand() {
    document.execCommand("InsertLineBreak");
    debugForDumpAsText("execInsertLineBreakCommand");
}
function insertLineBreakCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execInsertLineBreakCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execInsertLineBreakCommand();
    }
}

//-------------------------------------------------------------------------------------------------------
 
function execInsertParagraphCommand() {
    document.execCommand("InsertParagraph");
    debugForDumpAsText("execInsertParagraphCommand");
}
function insertParagraphCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execInsertParagraphCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execInsertParagraphCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execInsertNewlineInQuotedContentCommand() {
    document.execCommand("InsertNewlineInQuotedContent");
    debugForDumpAsText("execInsertNewlineInQuotedContentCommand");
}
function insertNewlineInQuotedContentCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execInsertNewlineInQuotedContentCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execInsertNewlineInQuotedContentCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execTypeCharacterCommand(c) {
    if (arguments.length == 0 || c == undefined || c.length == 0 || c.length > 1)
        c = 'x';
    document.execCommand("InsertText", false, c);
    debugForDumpAsText("execTypeCharacterCommand");
}
function typeCharacterCommand(c) {
    if (commandDelay > 0) {
        window.setTimeout(execTypeCharacterCommand, commandCount * commandDelay, c);
        commandCount++;
    }
    else {
        execTypeCharacterCommand(c);
    }
}

//-------------------------------------------------------------------------------------------------------

function execSelectAllCommand() {
    document.execCommand("SelectAll");
}
function selectAllCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execSelectAllCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execSelectAllCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execStrikethroughCommand() {
    document.execCommand("Strikethrough");
    debugForDumpAsText("execStrikethroughCommand");
}
function strikethroughCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execStrikethroughCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execStrikethroughCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execUndoCommand() {
    document.execCommand("Undo");
    debugForDumpAsText("execUndoCommand");
}
function undoCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execUndoCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execUndoCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execRedoCommand() {
    document.execCommand("Redo");
    debugForDumpAsText("execRedoCommand");
}
function redoCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execRedoCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execRedoCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execChangeRootSize() {
    document.getElementById("root").style.width = "600px";
}
function changeRootSize() {
    if (commandDelay > 0) {
        window.setTimeout(execChangeRootSize, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execChangeRootSize();
    }
}

//-------------------------------------------------------------------------------------------------------

function execCutCommand() {
    document.execCommand("Cut");
    debugForDumpAsText("execCutCommand");
}
function cutCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execCutCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execCutCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execCopyCommand() {
    document.execCommand("Copy");
    debugForDumpAsText("execCopyCommand");
}
function copyCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execCopyCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execCopyCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execPasteCommand() {
    document.execCommand("Paste");
    debugForDumpAsText("execPasteCommand");
}
function pasteCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execPasteCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execPasteCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execCreateLinkCommand(url) {
    document.execCommand("CreateLink", false, url);
    debugForDumpAsText("execCreateLinkCommand");
}
function createLinkCommand(url) {
    if (commandDelay > 0) {
        window.setTimeout(execCreateLinkCommand, commandCount * commandDelay, url);
        commandCount++;
    } else
        execCreateLinkCommand(url);
}

//-------------------------------------------------------------------------------------------------------

function execUnlinkCommand() {
    document.execCommand("Unlink");
    debugForDumpAsText("execUnlinkCommand");
}
function unlinkCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execUnlinkCommand, commandCount * commandDelay);
        commandCount++;
    } else
        execUnlinkCommand();
}

//-------------------------------------------------------------------------------------------------------

function execPasteAndMatchStyleCommand() {
    document.execCommand("PasteAndMatchStyle");
    debugForDumpAsText("execPasteAndMatchStyleCommand");
}
function pasteAndMatchStyleCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execPasteAndMatchStyleCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execPasteAndMatchStyleCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execDeleteCommand() {
    document.execCommand("Delete");
    debugForDumpAsText("execDeleteCommand");
}
function deleteCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execDeleteCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execDeleteCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function execForwardDeleteCommand() {
    document.execCommand("ForwardDelete");
    debugForDumpAsText("execForwardDeleteCommand");
}
function forwardDeleteCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execForwardDeleteCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execForwardDeleteCommand();
    }
}

//-------------------------------------------------------------------------------------------------------

function runEditingTest() {
    if (window.testRunner)
        testRunner.dumpEditingCallbacks();

    var elem = document.getElementById("test");
    var selection = window.getSelection();
    selection.setPosition(elem, 0);
    editingTest();
}

var dumpAsText = false;
var markupResultList = document.createElement('ol');

function runDumpAsTextEditingTest(enableCallbacks) {
    if (window.testRunner) {
         testRunner.dumpAsText();
         if (enableCallbacks)
            testRunner.dumpEditingCallbacks();
     }

    dumpAsText = true;

    var elem = document.getElementById("test");
    var selection = window.getSelection();
    selection.setPosition(elem, 0);
    editingTest();

    document.body.appendChild(markupResultList);
}

function debugForDumpAsText(name) {
    if (dumpAsText && document.getElementById("root")) {
        var newItem = document.createElement('li');
        newItem.appendChild(document.createTextNode(name+": "+document.getElementById("root").innerHTML));
        markupResultList.appendChild(newItem);
    }
}

//-------------------------------------------------------------------------------------------------------


function execBackColorCommand() {
    document.execCommand("BackColor", false, "Chartreuse");
    debugForDumpAsText('execBackColorCommand');
}
function backColorCommand() {
    if (commandDelay > 0) {
        window.setTimeout(execBackColorCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        execBackColorCommand();
    }
}


function execForeColorCommand(color) {
    document.execCommand("ForeColor", false, color);
    debugForDumpAsText('execForeColorCommand');
}
function foreColorCommand(color) {
    if (commandDelay > 0) {
        window.setTimeout(execForeColorCommand, commandCount * commandDelay, color);
        commandCount++;
    } else
        execForeColorCommand(color);
}

//-------------------------------------------------------------------------------------------------------


function runCommand(command, arg1, arg2) {
    document.execCommand(command,arg1,arg2);
}

function executeCommand(command,arg1,arg2) {
    if (commandDelay > 0) {
        window.setTimeout(runCommand, commandCount * commandDelay);
        commandCount++;
    }
    else {
        runCommand(command,arg1,arg2);
    }
}

