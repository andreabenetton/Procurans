// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef IREPEATABLE_H
#define IREPEATABLE_H

#include <QXmlStreamWriter>

namespace qoasis {

    class IRepeatable
	{
	public:
        IRepeatable(int repeat = 1);
        IRepeatable(const IRepeatable &obj);

        virtual QLatin1String repeatTag() = 0;

		int getRepeat() const;

        template<typename T> static int scanForwardForNotNull(int index, QVector<QSharedPointer<T>> v);
        template<typename T> static int scanBackwardForNotNull(int index, QVector<QSharedPointer<T>> v);

	protected:
        int repeat_;

        void readRepeat(QStringRef value);
        void writeRepeat(QXmlStreamWriter* writer);
	};

    // Static methods - they raise error if placed in cpp file
    template<typename T>
    int IRepeatable::scanForwardForNotNull(int index, QVector<QSharedPointer<T>> v)
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
    int IRepeatable::scanBackwardForNotNull(int index, QVector<QSharedPointer<T>> v)
    {
        Q_ASSERT(index >= 0);
        int last_in_container = v.length()-1;
        // the index can be legally larger than container lenght because of repeating items;
        // in that case start from last position in the container
        index = (index > last_in_container) ? last_in_container : index;

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
