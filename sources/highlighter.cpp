#include <QtWidgets>
#include <QtCore>
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QColor colorMath = QColor(0x00,0x80, 0x00);
    QColor ColorKeyword=QColor(0x00, 0x00, 0xCC);
    QColor colorOther=QColor(0x60, 0x44, 0xA7);
    QColor colorComment=QColor(0xA8,0x30,0x06);
    QColor colorItem=QColor(0x82,0x00,0x81);
    HighlightingRule rule;

    keywordFormat.setForeground(ColorKeyword);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns= QString("section,subsection,subsubsection,chapter,part,paragraph,subparagraph,section\\*,subsection\\*,subsubsection\\*,chapter\\*,part\\*,paragraph\\*,subparagraph\\*,label,includegraphics,includegraphics\\*,include,input,begin,end").split(",");

    otherFormat.setForeground(colorOther);
    otherFormat.setFontWeight(QFont::Bold);
    QStringList otherPatterns;
    otherPatterns << "text" << "textit" << "textbf" << "textrm" << "emph" << "underline";


    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\\\("+pattern+")");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    foreach (const QString &pattern, otherPatterns) {
        rule.pattern = QRegExp("\\\\("+pattern+")");
        rule.format = otherFormat;
        highlightingRules.append(rule);
    }

    itemFormat.setForeground(colorItem);
    rule.pattern = QRegExp("\\\\item");
    rule.format = itemFormat;
    highlightingRules.append(rule);

    commentFormat.setForeground(colorComment);
    rule.pattern = QRegExp("\\%[^\n]*");
    rule.format = commentFormat;
    highlightingRules.append(rule);

    inlineMathFormat.setForeground(colorMath);
    inlineMathLimit = QRegExp("\\$");

    multiLineMathFormat.setForeground(colorMath);
    mathStartExpression = QRegExp("\\\\(\\[)");
    mathEndExpression = QRegExp("\\\\(\\])");
}

void Highlighter::highlightBlock(const QString &text)
{
    // Toutes les règles simples
    foreach (const HighlightingRule &rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = expression.indexIn(text, index + length);
            }
    }

    // Les math inline
    int index = inlineMathLimit.indexIn(text);
    while (index >= 0) {
        int stopIndex = inlineMathLimit.indexIn(text, index+1);
        int length;
        if (stopIndex == -1) {
            length = text.length() - index;
        }
        else length = stopIndex - index+1;

        setFormat(index, length, inlineMathFormat);
        index = inlineMathLimit.indexIn(text, index + length + 1);
    }


    // Les blocs de math
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
