#include <QtWidgets>
#include <QtCore>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QColor colorMath = QColor(0x00,0x80, 0x00);
    QColor ColorKeyword=QColor(0x00, 0x00, 0xCC);
    HighlightingRule rule;

    keywordFormat.setForeground(ColorKeyword);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns= QString("section,subsection,subsubsection,chapter,part,paragraph,subparagraph,section\\*,subsection\\*,subsubsection\\*,chapter\\*,part\\*,paragraph\\*,subparagraph\\*,label,includegraphics,includegraphics\\*,include,input,begin,end").split(",");

    //keywordPatterns << "\\]";
    //keywordPatterns << "\\{\\w+";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\\\("+pattern+")");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    inlineMathFormat.setForeground(colorMath);
    rule.pattern = QRegExp("\\$(.*)\\$");
    rule.format = inlineMathFormat;
    highlightingRules.append(rule);

    multiLineMathFormat.setForeground(colorMath);
    mathStartExpression = QRegExp("\\\\(\\[)");
    mathEndExpression = QRegExp("\\\\(\\])");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = mathStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = mathEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + mathEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineMathFormat);
        startIndex = mathStartExpression.indexIn(text, startIndex + commentLength);
    }
}
