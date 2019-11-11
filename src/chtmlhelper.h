#ifndef CHTMLHELPER_H
#define CHTMLHELPER_H

#include <QString>
#include <QMap>

enum eTagType{
    open_tag,
    close_tag,
    open_close_tag
};

typedef struct _TagInfo {
    int startPos;
    int endPos;
    eTagType tagType;
} TagInfo;

class CHtmlHelper
{
public:
    CHtmlHelper();
    bool parse(const QByteArray& strHtml, int startPos = 0);
    const QMap<int, TagInfo>& GetTagMap() { return m_tagMap; }
private:
    QMap<int, TagInfo> m_tagMap;
};

#endif // CHTMLHELPER_H
