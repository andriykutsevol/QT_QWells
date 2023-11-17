#ifndef QW_INPUT_TEXTEDIT_H
#define QW_INPUT_TEXTEDIT_H

#include <QTextEdit>
#include <QMouseEvent>

#include <QTextCursor>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextDocument>

class qw_input_TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit qw_input_TextEdit(QWidget *parent = 0);
    int block_id;
    QTextBlock *block;
    QTextCursor *cursor_selectBlockUnder;


signals:
    //void block_selected(int);
    void block_selected();

public slots:

protected:
        virtual void mousePressEvent( QMouseEvent* event );

};

#endif // QW_INPUT_TEXTEDIT_H
