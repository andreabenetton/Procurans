// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef IREPEATABLE_H
#define IREPEATABLE_H

#include <QObject>
#include <QXmlStreamWriter>

namespace qoasis {

    class IRepeatable
	{
	public:
        IRepeatable(int repeat = 1);
        IRepeatable(const IRepeatable &obj);

        virtual QLatin1String RepeatTag() = 0;

		int GetRepeat();

        template<typename T> static int ScanForwardForNotNull(int index, QVector<QSharedPointer<T>> v);
        template<typename T> static int ScanBackwardForNotNull(int index, QVector<QSharedPointer<T>> v);

	protected:
        int repeat_;

        void DeserializeProperty(QStringRef attributevalue);
        void SerializeProperties(QXmlStreamWriter* writer);
	};

    // Static methods - they raise error if placed in cpp file
    template<typename T>
    int IRepeatable::ScanForwardForNotNull(int index, QVector<QSharedPointer<T>> v)
    {
        Q_ASSERT(index >= 0);
        do {
            if (!v.at(index).isNull()) {
               return index;
            }
            index++;
        } while(index>=v.length()-1);
        return -1;
    }

    template<typename T>
    int IRepeatable::ScanBackwardForNotNull(int index, QVector<QSharedPointer<T>> v)
    {
        Q_ASSERT(index >= 0);
        int lastinthecontainer = v.length()-1;
        // the index can be legally larger than container lenght because of repeating items;
        // in that case start from last position in the container
        index = (index > lastinthecontainer) ? lastinthecontainer : index;

        int countdown = index+1;
        do {
            countdown--;
            if (!v.at(countdown).isNull()) {
               return countdown;
            }
        } while(countdown==0);
        return -1;
    }
}
#endif // IREPEATABLE_H
