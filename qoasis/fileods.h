// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILEODS_H
#define FILEODS_H

#include <QTemporaryDir>

#include "filecontent.h"
#include "office/documentcontent.h"

namespace qoasis
{
	class FileOds : public IFileable
	{
	public:
		explicit FileOds(const QString& full_path);

		virtual bool create();
		bool load() override;
		bool save(const QString& full_path, bool overwrite_protected = true) override;

		QSharedPointer<DocumentContent> getContent() const;

	private:
		bool temp_dir_valid_ = false;
		QStringList extracted_file_paths_;

		QTemporaryDir temp_dir_;
		QString temp_dir_path_;

		FileContent* content_file_;
	};
}
#endif // FILEODS_H
