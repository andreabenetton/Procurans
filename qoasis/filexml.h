// Copyright 2019 - 2020, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FILEXML_H
#define FILEXML_H

#include <QFile>
#include <QXmlStreamWriter>

#include "ifileable.h"

namespace qoasis
{
	class FileXml : public IFileable
	{
	public:
		explicit FileXml(const QString& full_path);

		bool load() override;
		bool save(const QString& full_path, bool overwrite_protected = true) override;
		bool save() override;
		virtual QLatin1String instanceFileName() = 0;

	protected:
		virtual QLatin1String getRootTag() = 0;
		virtual void read(QXmlStreamReader& xml) = 0;
		virtual void write(QXmlStreamWriter* xml) = 0;
		bool isStartElementNamed(QXmlStreamReader& xml, const QString& token_name);
		bool isNotEndElementNamed(QXmlStreamReader& xml, const QString& token_name);
	};
}
#endif // FILEXML_H
