// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>

namespace Ods {
	bool IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName);
	bool IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName);
}
#endif // FUNCTIONS_H
