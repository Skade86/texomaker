#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{

    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp mathStartExpression;
    QRegExp mathEndExpression;
    QRegExp inlineMathLimit;

    QTextCharFormat keywordFormat;
    QTextCharFormat otherFormat;
    QTextCharFormat inlineMathFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat multiLineMathFormat;
    QTextCharFormat itemFormat;
    QTextCharFormat functionFormat;

    int previousIndex;
};

#endif // HIGHLIGHTER_H
