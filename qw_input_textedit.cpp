#include "qw_input_textedit.h"

#include <QDebug>

#include <QTextCursor>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextDocument>


qw_input_TextEdit::qw_input_TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    this->block_id = 0;
}


void qw_input_TextEdit::mousePressEvent(QMouseEvent* event){

   cursor_selectBlockUnder = new QTextCursor;
   *cursor_selectBlockUnder = cursorForPosition(event->pos());

  // *cursor_selectBlockUnder->select(QTextCursor::BlockUnderCursor);
    //emit block_selected(cursor.blockNumber());
    this->block_id = cursor_selectBlockUnder->blockNumber();
    cursor_selectBlockUnder->select(QTextCursor::BlockUnderCursor);
    emit block_selected();

//    cursor.select(QTextCursor::BlockUnderCursor);
//    cursor.removeSelectedText();
//    qDebug()<< "line count" << cursor.block().lineCount();
//    qDebug()<< "block count"<< this->document()->blockCount();
}
