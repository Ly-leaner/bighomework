#include "plistreader.h"
#include <QIODevice>
#include <QVariant>
#include <QMap>

PListReader::PListReader()
{}

bool PListReader::read(QIODevice *device)
{
    m_xmlReader.setDevice(device);  //����QXmlStreamReader�Ľ����豸

    if (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == "plist" && m_xmlReader.attributes().value("version") == "1.0")
            readPList();
        else
            m_xmlReader.raiseError("The file is not an PList version 1.0 file.");
    }

    return m_xmlReader.error();
}

const QList<QVariant> PListReader::data() const
{
    return m_data;
}

QString PListReader::errorString() const
{
    return QString("%1\nLine %2, column %3")
            .arg(m_xmlReader.errorString())
            .arg(m_xmlReader.lineNumber())
            .arg(m_xmlReader.columnNumber());
}

void PListReader::readPList()
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "plist");

    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == "array")
            readArray(m_data);
        else if (m_xmlReader.name() == "dict")
            readDict(m_data);
        else
            m_xmlReader.skipCurrentElement();
    }
}

void PListReader::readArray(QList<QVariant> &array)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "array");

    while (m_xmlReader.readNextStartElement())
    {
        if (m_xmlReader.name() == "array")
        {
            QList<QVariant> subArray;
            readArray(subArray);
            array.push_back(subArray);
        }
        else if (m_xmlReader.name() == "dict")
        {
            readDict(array);
        }
        else
        {
            m_xmlReader.skipCurrentElement();
        }
    }
}

void PListReader::readDict(QList<QVariant> &array)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "dict");

    QMap<QString, QVariant> dict;
    while (m_xmlReader.readNextStartElement())
    {
        // ����ֻ����key,��readKey��,һ��Ĭ�϶�ȡһ�Լ�ֵ
        if (m_xmlReader.name() == "key")
            readKey(dict);
        else
            m_xmlReader.skipCurrentElement();
    }

    array.push_back(dict);
}

void PListReader::readKey(QMap<QString, QVariant> &dict)
{
    Q_ASSERT(m_xmlReader.isStartElement() && m_xmlReader.name() == "key");

    // ����һ�ζ�ȡһ����ֵ��
    QString key = m_xmlReader.readElementText();
    Q_ASSERT(m_xmlReader.readNextStartElement());
    QString value = m_xmlReader.readElementText();

    dict.insertMulti(key, value);
    /*
     * Inserts a new item with the key key and a value of value.
     * If there is already an item with the same key in the map,
     * this function will simply create a new one.
     * (This behavior is different from insert(), which overwrites the value of an existing item.)
    */
}
