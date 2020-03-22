// Copyright 2019 - 2020, Andrea Benetton and the Procurans contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include "functions.h"

namespace Ods {
    bool IsStartElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return ((xml.tokenType() == QXmlStreamReader::StartElement) && (xml.qualifiedName() == tokenName));
    }

    bool IsNotEndElementNamed(QXmlStreamReader& xml, const QString& tokenName)
    {
        return !((xml.tokenType() == QXmlStreamReader::EndElement) && (xml.qualifiedName() == tokenName));
    }
}