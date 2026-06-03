// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "qoasis/tag.h"

using qoasis::Tag;

namespace
{
	// Parse the first start element in `xml` via QXmlStreamReader, build a
	// generic Tag from it, write it back out via QXmlStreamWriter, and
	// return the serialized bytes (no document prolog, no auto-formatting).
	QByteArray roundtrip(const QByteArray& xml, QSharedPointer<Tag>* outTag = nullptr)
	{
		QXmlStreamReader reader(xml);
		while (!reader.atEnd() && reader.tokenType() != QXmlStreamReader::StartElement)
			reader.readNext();
		Q_ASSERT(reader.tokenType() == QXmlStreamReader::StartElement);

		QSharedPointer<Tag> tag = Tag::buildGeneric(reader);
		if (outTag) *outTag = tag;

		QByteArray out;
		QXmlStreamWriter writer(&out);
		writer.setAutoFormatting(false);
		tag->write(&writer);
		return out;
	}
}

class TestTag : public QObject
{
	Q_OBJECT

private slots:
	void attributeSurvivesRoundTrip();
	void namespaceDeclarationSurvives();
	void inlineTextSurvivesAndPlainTextReports();
	void nestedSubtagsRoundTripInOrder();
	void mixedContentPlainTextConcatenates();
	void emptyElementWrittenSelfClosing();
	void plainTextRecursesIntoDescendants();
};

void TestTag::attributeSurvivesRoundTrip()
{
	const QByteArray in = "<foo bar=\"baz\"/>";
	const QByteArray out = roundtrip(in);
	QVERIFY2(out.contains("<foo"), out.constData());
	QVERIFY2(out.contains("bar=\"baz\""), out.constData());
}

void TestTag::namespaceDeclarationSurvives()
{
	const QByteArray in = "<foo xmlns:n=\"urn:ns\" n:bar=\"baz\"/>";
	const QByteArray out = roundtrip(in);
	QVERIFY2(out.contains("xmlns:n=\"urn:ns\""), out.constData());
	QVERIFY2(out.contains("n:bar=\"baz\""), out.constData());
}

void TestTag::inlineTextSurvivesAndPlainTextReports()
{
	QSharedPointer<Tag> tag;
	const QByteArray out = roundtrip("<foo>hello</foo>", &tag);
	QVERIFY2(out.contains("hello"), out.constData());
	QCOMPARE(tag->plainText(), QStringLiteral("hello"));
}

void TestTag::nestedSubtagsRoundTripInOrder()
{
	const QByteArray in = "<foo><bar/><baz/></foo>";
	const QByteArray out = roundtrip(in);
	const int barAt = out.indexOf("<bar");
	const int bazAt = out.indexOf("<baz");
	QVERIFY2(barAt >= 0, out.constData());
	QVERIFY2(bazAt > barAt, out.constData());
}

void TestTag::mixedContentPlainTextConcatenates()
{
	// Characters tokens around <bar> are kept by loopToReadSubtag; the
	// generic Tag model concatenates them into a single inline_text_,
	// then plainText() prepends that to bar's own plainText("middle").
	QSharedPointer<Tag> tag;
	roundtrip("<foo>start <bar>middle</bar> end</foo>", &tag);
	const QString text = tag->plainText();
	QVERIFY2(text.contains(QStringLiteral("start")), qPrintable(text));
	QVERIFY2(text.contains(QStringLiteral("middle")), qPrintable(text));
	QVERIFY2(text.contains(QStringLiteral("end")), qPrintable(text));
}

void TestTag::emptyElementWrittenSelfClosing()
{
	const QByteArray out = roundtrip("<foo/>");
	// QXmlStreamWriter emits self-closing for empty elements; allow either
	// "<foo/>" or "<foo />" but reject the explicit close pair.
	QVERIFY2(!out.contains("</foo>"), out.constData());
	QVERIFY2(out.contains("<foo"), out.constData());
}

void TestTag::plainTextRecursesIntoDescendants()
{
	QSharedPointer<Tag> tag;
	roundtrip("<a><b>X<c>Y</c></b></a>", &tag);
	const QString text = tag->plainText();
	QVERIFY2(text.contains(QStringLiteral("X")), qPrintable(text));
	QVERIFY2(text.contains(QStringLiteral("Y")), qPrintable(text));
}

QTEST_MAIN(TestTag)
#include "test_tag.moc"
