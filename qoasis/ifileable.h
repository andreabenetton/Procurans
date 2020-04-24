// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef IFILEABLE_H
#define IFILEABLE_H

namespace qoasis
{
	class IFileable
	{
	public:
		IFileable(const QString& full_path);

		virtual bool exist();
		virtual bool load() = 0;
		virtual bool save(const QString& full_path, bool overwrite_protected = true) = 0;
		virtual bool save();

	protected:
		QString full_path_;
	};
}
#endif // IFILEABLE_H
