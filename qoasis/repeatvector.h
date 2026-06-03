// Copyright 2020 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef REPEATVECTOR_H
#define REPEATVECTOR_H

#include <QDebug>
#include <QXmlStreamWriter>

namespace qoasis
{
	/**
	 * \brief A vector that manages internally the repetition of its elements
	 * \tparam T an IRepeatable subclass
	 */
	template <class T>
	class RepeatVector
	{
	public:
		enum Direction { Forward = 1, Backward = -1 };

		enum Type { Base, Repeat, Null };

		const int MAXREPEAT = 1000;

		RepeatVector();
		RepeatVector(int array_size, QSharedPointer<T> items[]);

		void append(QSharedPointer<T> value);
		void append(int array_size, QSharedPointer<T> items[]);
		QSharedPointer<T> at(int i) const;
		void insert(int index, QSharedPointer<T> value);
		int lastNotEmpty() const;
		void remove(int index);
		void remove(int index, int count);
		void removeEndingEmpty();
		void replace(int index, QSharedPointer<T> value);
		int scanForBaseItem(int index, Direction direction) const;
		int size() const;

		QSharedPointer<T> operator[](int index);

		void write(QXmlStreamWriter* writer);

	private:
		QVector<QSharedPointer<T>> vector_;

		void insertWithRepeats(int index, QSharedPointer<T> value);
	};

	// Constructors
	template <class T>
	RepeatVector<T>::RepeatVector()
	{
		vector_ = QVector<QSharedPointer<T>>(0, QSharedPointer<T>(nullptr));
	}

	template <class T>
	RepeatVector<T>::RepeatVector(int array_size, QSharedPointer<T> items[]) : RepeatVector()
	{
		this.append(array_size, items);
	}

	template <class T>
	void RepeatVector<T>::append(const QSharedPointer<T> value)
	{
		Q_ASSERT(!value.isNull());
		if (value->getRepeat() < MAXREPEAT) {
			vector_.append(value);
			for (int i = 1; i < value->getRepeat(); i++) {
				vector_.append(QSharedPointer<T>(nullptr));
			}
		}
		else {
			qDebug() << "Repeat vector, skipped item with repeat: " << value->getRepeat();
		}
	}

	template <class T>
	void RepeatVector<T>::append(int array_size, QSharedPointer<T> items[])
	{
		Q_ASSERT(array_size >= 0);
		for (int i = 0; i < array_size; i++) {
			append(items[i]);
		}
	}

	template <class T>
	QSharedPointer<T> RepeatVector<T>::at(int i) const
	{
		Q_ASSERT(i >= 0);
		if (i >= vector_.size()) {
			return QSharedPointer<T>(nullptr);
		}
		return vector_.at(i);
	}

	template <class T>
	void RepeatVector<T>::insert(int index, QSharedPointer<T> value)
	{
		Q_ASSERT(index >= 0);
		Q_ASSERT(!value.isNull());

		if (value->getRepeat() < MAXREPEAT) {
			if (index == vector_.size()) {
				vector_.append(value);
				return;
			}
			if (index > vector_.size()) {
				const int previous_size = vector_.size();
				QSharedPointer<T> default_item = T::placeholder(index - previous_size);
				append(default_item);
				append(value);
				return;
			}
			if (vector_.at(index).isNull()) {
				const auto base_index = scanForBaseItem(index, Backward);
				// reduce repetitions of the base element
				const int previous_base_repeat = vector_.at(base_index)->getRepeat();
				const int new_base_repeat = index - base_index - 1;
				vector_.at(base_index)->setRepeat(new_base_repeat);
				// insert the new element, filling repetitions in the case
				vector_.insert(index, value);
				for (int j = 1; j < value->getRepeat(); j++) {
					vector_.insert(index + j, QSharedPointer<T>(nullptr));
				}
				// replace the following element with a placeholder
				QSharedPointer<T> default_item = T::placeholder(previous_base_repeat - new_base_repeat);
				vector_.replace(index + 1, default_item);
				return;
			}

			insertWithRepeats(index, value);
		}
	}

	/**
	 * \brief Search for the last non-empty item, "empty" as defined by the item itself.
	 * \return The index of last non-empty item.
	 */
	template <class T>
	int RepeatVector<T>::lastNotEmpty() const
	{
		for (int count = vector_.size() - 1; count >= 0; count--)
		{
			if (!vector_.at(count).isNull()) {
				if (!vector_.at(count)->isEmpty()) {
					return count;
				}
			}
		}
		return -1;
	}

	template <class T>
	void RepeatVector<T>::remove(int index)
	{
		Q_ASSERT(index >= 0);
		if (index >= vector_.size()) {
			return;
		}
		if (vector_.at(index).isNull()) {
			const auto base_index = scanForBaseItem(index, Backward);
			vector_.at(base_index)->offsetRepeatBy(-1);
			vector_.remove(index);
			return;
		}
		if (vector_.at(index)->getRepeat() > 1) {
			vector_.at(index)->offsetRepeatBy(-1);
			vector_.remove(index + 1);
			return;
		}
		vector_.remove(index);
	}

	template <class T>
	void RepeatVector<T>::remove(int index, int count)
	{
		Q_ASSERT(index >= 0);
		Q_ASSERT(count >= 1);
		do {
			this->remove(index);
			--count;
		}
		while (count >= 0);
	}

	template <class T>
	void RepeatVector<T>::removeEndingEmpty()
	{
		int last = lastNotEmpty();
		int size = vector_.size() - 1;
		qDebug() << "Repeat vector, removing ending empty, from: " << last << "to: " << size;
		for (int i = size; i > last; i--)
		{
			vector_.remove(i);
		}
	}

	template <class T>
	void RepeatVector<T>::replace(int index, QSharedPointer<T> value)
	{
		Q_ASSERT(index >= 0);
		Q_ASSERT(!value.isNull());

		if (value->getRepeat() < MAXREPEAT) {
			if (index == vector_.size()) {
				this.append(value);
				return;
			}
			if (index > vector_.size()) {
				const int previous_size = vector_.size();
				QSharedPointer<T> default_item = T::placeholder(index - previous_size);
				this.append(default_item);
				this.append(value);
				return;
			}
			if (vector_.at(index).isNull()) {
				const auto base_index = scanForBaseItem(index, Backward);
				// reduce repetitions of the base element
				const int previous_base_repeat = vector_.at(base_index)->getRepeat();
				const int new_base_repeat = index - base_index - 1;
				vector_.at(base_index)->setRepeat(new_base_repeat);
				// insert the new element, filling repetitions in the case
				vector_.remove(index);
				vector_.insertWithRepeats(index, value);
				// replace the following element with a placeholder
				QSharedPointer<T> default_item = T::placeholder(previous_base_repeat - new_base_repeat);
				vector_.replace(index + 1, default_item);
				return;
			}

			vector_.remove(index);
			vector_.insertWithRepeats(index, value);
		}
	}

	/**
	 * \brief Look for the item that is base of the repetition.
	 * \param index The index from which the scan begins.
	 * \param direction The direction to scan.
	 * \return The index of the base item.
	 */
	template <class T>
	int RepeatVector<T>::scanForBaseItem(int index, Direction direction) const
	{
		Q_ASSERT(index >= 0);
		index = (index > vector_.size() - 1) ? vector_.size() - 1 : index;
		do {
			if (!vector_.at(index).isNull()) {
				return index;
			}
			index += direction;
		}
		while (direction == Direction::Forward ? index >= vector_.size() - 1 : index <= 0);
		return -1;
	}

	template <class T>
	int RepeatVector<T>::size() const
	{
		int last_base_index = scanForBaseItem(vector_.size() - 1, Direction::Backward);
		return last_base_index + vector_[last_base_index]->getRepeat() - 1;
	}

	template <class T>
	QSharedPointer<T> RepeatVector<T>::operator[](int index)
	{
		Q_ASSERT(index >= 0);
		if (index >= vector_.size()) {
			return QSharedPointer<T>(nullptr);
		}
		return vector_[index];
	}

	/**
	 * \brief Serialize the vector in a QXmlStreamWriter
	 * \param writer A pointer to the stream writer
	 */
	template <class T>
	void RepeatVector<T>::write(QXmlStreamWriter* writer)
	{
		for (int i = 0; i < vector_.size(); ++i) {
			if (!vector_.at(i).isNull()) {
				vector_.at(i)->write(writer);
			}
		}
	}

	/**
	 * \brief Insert a item, adding repeated item placeholder
	 * \param index the position in the vector
	 * \param value the item to insert
	 */
	template <class T>
	void RepeatVector<T>::insertWithRepeats(int index, QSharedPointer<T> value)
	{
		vector_.insert(index, value);
		for (int j = 1; j < value->getRepeat(); j++) {
			vector_.insert(index + j, QSharedPointer<T>(nullptr));
		}
	};
}
#endif // REPEATVECTOR_H
