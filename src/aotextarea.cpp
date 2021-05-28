#include "aotextarea.h"

#include <QDateTime>
#include <QDebug>
#include <QRegExp>
#include <QScrollBar>
#include <QTextCursor>

const QRegExp AOTextArea::omnis_dank_url_regex = QRegExp("\\b(https?://\\S+\\.\\S+)\\b");

AOTextArea::AOTextArea(QWidget *p_parent) : QTextBrowser(p_parent)
{}

void AOTextArea::append_chatmessage(QString p_name, QString p_message)
{
  const QTextCursor old_cursor = this->textCursor();
  const int old_scrollbar_value = this->verticalScrollBar()->value();
  const bool is_scrolled_down = old_scrollbar_value == this->verticalScrollBar()->maximum();

  this->moveCursor(QTextCursor::End);

  this->append("");
  this->insertHtml(QString("[%1] ").arg(QDateTime::currentDateTime().toString("hh:mm")));
  this->insertHtml("<b>" + p_name.toHtmlEscaped() + "</b>:&nbsp;");

  // cheap workarounds ahoy
  p_message += " ";
  QString result = p_message.toHtmlEscaped().replace("\n", "<br>");
  result = result.replace(omnis_dank_url_regex, "<a href='\\1'>\\1</a>");

  this->insertHtml(result);

  this->auto_scroll(old_cursor, old_scrollbar_value, is_scrolled_down);
}

void AOTextArea::append_error(QString p_message)
{
  const QTextCursor old_cursor = this->textCursor();
  const int old_scrollbar_value = this->verticalScrollBar()->value();
  const bool is_scrolled_down = old_scrollbar_value == this->verticalScrollBar()->maximum();

  this->moveCursor(QTextCursor::End);

  this->append("");
  this->insertHtml(QString("[%1] ").arg(QDateTime::currentDateTime().toString("hh:mm")));

  p_message += " ";
  QString result = p_message.replace("\n", "<br>");
  result = result.replace(omnis_dank_url_regex, "<a href='\\1'>\\1</a>");

  this->insertHtml("<font color='red'>" + result + "</font>");

  this->auto_scroll(old_cursor, old_scrollbar_value, is_scrolled_down);
}

void AOTextArea::auto_scroll(QTextCursor old_cursor, int old_scrollbar_value, bool is_scrolled_down)
{
  if (old_cursor.hasSelection() || !is_scrolled_down)
  {
    // The user has selected text or scrolled away from the bottom: maintain
    // position.
    this->setTextCursor(old_cursor);
    this->verticalScrollBar()->setValue(old_scrollbar_value);
  }
  else
  {
    // The user hasn't selected any text and the scrollbar is at the bottom:
    // scroll to the bottom.
    this->moveCursor(QTextCursor::End);
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum());
  }
}
