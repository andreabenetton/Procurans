// Copyright 2026 - 2026, the QOasis contributors
// SPDX-License-Identifier: AGPL-3.0-only

#include <QtTest>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QTemporaryDir>

#include "qoasis/fileods.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"
#include "quazip/quazipfileinfo.h"
#include "quazip/quazipnewinfo.h"

namespace
{
	const QByteArray kMimetype =
		QByteArrayLiteral("application/vnd.oasis.opendocument.spreadsheet");

	const QByteArray kManifest = QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<manifest:manifest"
		" xmlns:manifest=\"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0\""
		" manifest:version=\"1.2\">\n"
		"  <manifest:file-entry manifest:full-path=\"/\""
		" manifest:version=\"1.2\""
		" manifest:media-type=\"application/vnd.oasis.opendocument.spreadsheet\"/>\n"
		"  <manifest:file-entry manifest:full-path=\"content.xml\""
		" manifest:media-type=\"text/xml\"/>\n"
		"</manifest:manifest>\n");
}

class TestFileOdsRoundtrip : public QObject
{
	Q_OBJECT

private slots:
	void initTestCase();
	void mimetypeStoredFirst();                       // commit 09d817d
	void unmodeledSpreadsheetSubtagsPreserved();      // commit cb551cc
	void inlineTextPreservedInGenericTag();           // commit 96c8c3c
	void coveredTableCellKeepsRowOrder();             // current commit

private:
	QTemporaryDir work_;

	void writeOds(const QString& outPath, const QByteArray& contentXml);
	QByteArray readArchiveEntry(const QString& path, const QString& name);
	void firstEntryInfo(const QString& path, QString* name, quint16* method);
};

void TestFileOdsRoundtrip::initTestCase()
{
	QVERIFY(work_.isValid());
}

void TestFileOdsRoundtrip::writeOds(const QString& outPath,
                                    const QByteArray& contentXml)
{
	QuaZip zip(outPath);
	QVERIFY(zip.open(QuaZip::mdCreate));

	zip.setDataDescriptorWritingEnabled(false);
	QuaZipFile m(&zip);
	QuaZipNewInfo mi(QStringLiteral("mimetype"));
	mi.uncompressedSize = static_cast<ulong>(kMimetype.size());
	QVERIFY(m.open(QIODevice::WriteOnly, mi, nullptr, 0, 0, 0));
	QCOMPARE(m.write(kMimetype), qint64(kMimetype.size()));
	m.close();

	zip.setDataDescriptorWritingEnabled(true);
	QuaZipFile c(&zip);
	QVERIFY(c.open(QIODevice::WriteOnly,
	               QuaZipNewInfo(QStringLiteral("content.xml"))));
	QCOMPARE(c.write(contentXml), qint64(contentXml.size()));
	c.close();

	QuaZipFile mf(&zip);
	QVERIFY(mf.open(QIODevice::WriteOnly,
	                QuaZipNewInfo(QStringLiteral("META-INF/manifest.xml"))));
	QCOMPARE(mf.write(kManifest), qint64(kManifest.size()));
	mf.close();

	zip.close();
	QCOMPARE(zip.getZipError(), 0);
}

QByteArray TestFileOdsRoundtrip::readArchiveEntry(const QString& path,
                                                  const QString& name)
{
	QuaZip zip(path);
	if (!zip.open(QuaZip::mdUnzip)) return {};
	if (!zip.setCurrentFile(name)) return {};
	QuaZipFile f(&zip);
	if (!f.open(QIODevice::ReadOnly)) return {};
	const QByteArray data = f.readAll();
	f.close();
	zip.close();
	return data;
}

void TestFileOdsRoundtrip::firstEntryInfo(const QString& path,
                                          QString* name, quint16* method)
{
	*name = QString();
	*method = 0xFFFF;
	QuaZip zip(path);
	QVERIFY(zip.open(QuaZip::mdUnzip));
	QVERIFY(zip.goToFirstFile());
	QuaZipFileInfo info;
	QVERIFY(zip.getCurrentFileInfo(&info));
	*name = info.name;
	*method = info.method;
	zip.close();
}

void TestFileOdsRoundtrip::mimetypeStoredFirst()
{
	const QString in = work_.path() + QStringLiteral("/in.ods");
	const QString out = work_.path() + QStringLiteral("/out_mime.ods");

	writeOds(in, QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<office:document-content"
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" office:version=\"1.2\">\n"
		"  <office:body><office:spreadsheet/></office:body>\n"
		"</office:document-content>\n"));

	qoasis::FileOds f(in);
	QVERIFY(f.load());
	QVERIFY(f.save(out, false));

	QString first;
	quint16 method = 0xFFFF;
	firstEntryInfo(out, &first, &method);
	QCOMPARE(first, QStringLiteral("mimetype"));
	QCOMPARE(method, quint16(0));   // 0 = Z_STORED
}

void TestFileOdsRoundtrip::unmodeledSpreadsheetSubtagsPreserved()
{
	const QString in = work_.path() + QStringLiteral("/in.ods");
	const QString out = work_.path() + QStringLiteral("/out_subtags.ods");

	writeOds(in, QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<office:document-content"
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" office:version=\"1.2\">\n"
		"  <office:body><office:spreadsheet>\n"
		"    <table:calculation-settings table:case-sensitive=\"true\"/>\n"
		"    <table:content-validations>\n"
		"      <table:content-validation table:name=\"val1\"/>\n"
		"    </table:content-validations>\n"
		"  </office:spreadsheet></office:body>\n"
		"</office:document-content>\n"));

	qoasis::FileOds f(in);
	QVERIFY(f.load());
	QVERIFY(f.save(out, false));

	const QByteArray content = readArchiveEntry(out, QStringLiteral("content.xml"));
	QVERIFY2(content.contains("table:calculation-settings"), content.constData());
	QVERIFY2(content.contains("table:content-validations"), content.constData());
	QVERIFY2(content.contains("val1"), content.constData());
}

void TestFileOdsRoundtrip::inlineTextPreservedInGenericTag()
{
	const QString in = work_.path() + QStringLiteral("/in.ods");
	const QString out = work_.path() + QStringLiteral("/out_text.ods");

	// table:iteration is unmodeled by qoasis. Its inline text content used to
	// be silently dropped by Tag::loopToReadSubtag (Characters events ignored).
	writeOds(in, QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<office:document-content"
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" office:version=\"1.2\">\n"
		"  <office:body><office:spreadsheet>\n"
		"    <table:calculation-settings>"
		"<table:iteration>XYZmarker</table:iteration>"
		"</table:calculation-settings>\n"
		"  </office:spreadsheet></office:body>\n"
		"</office:document-content>\n"));

	qoasis::FileOds f(in);
	QVERIFY(f.load());
	QVERIFY(f.save(out, false));

	const QByteArray content = readArchiveEntry(out, QStringLiteral("content.xml"));
	QVERIFY2(content.contains("XYZmarker"), content.constData());
}

void TestFileOdsRoundtrip::coveredTableCellKeepsRowOrder()
{
	const QString in = work_.path() + QStringLiteral("/in.ods");
	const QString out = work_.path() + QStringLiteral("/out_covered.ods");

	// A row that mixes cell / spanning cell / covered / cell. Pre-fix,
	// covered cells fell through to Tag::readSubtag and serialized AFTER
	// all real cells, shifting the column layout.
	writeOds(in, QByteArrayLiteral(
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<office:document-content"
		" xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
		" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
		" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\""
		" office:version=\"1.2\">\n"
		"  <office:body><office:spreadsheet><table:table table:name=\"S\">\n"
		"    <table:table-row>\n"
		"      <table:table-cell office:value-type=\"string\">"
		"<text:p>FIRST</text:p></table:table-cell>\n"
		"      <table:table-cell office:value-type=\"string\""
		" table:number-columns-spanned=\"2\">"
		"<text:p>SPAN</text:p></table:table-cell>\n"
		"      <table:covered-table-cell/>\n"
		"      <table:table-cell office:value-type=\"string\">"
		"<text:p>LAST</text:p></table:table-cell>\n"
		"    </table:table-row>\n"
		"  </table:table></office:spreadsheet></office:body>\n"
		"</office:document-content>\n"));

	qoasis::FileOds f(in);
	QVERIFY(f.load());
	QVERIFY(f.save(out, false));

	const QByteArray content = readArchiveEntry(out, QStringLiteral("content.xml"));
	const int first   = content.indexOf("FIRST");
	const int span    = content.indexOf("SPAN");
	const int covered = content.indexOf("covered-table-cell");
	const int last    = content.indexOf("LAST");
	QVERIFY2(first   >= 0, content.constData());
	QVERIFY2(span    >  first,   content.constData());
	QVERIFY2(covered >  span,    content.constData());
	QVERIFY2(last    >  covered, content.constData());
}

QTEST_MAIN(TestFileOdsRoundtrip)
#include "test_fileods_roundtrip.moc"
