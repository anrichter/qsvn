#include <QtTest/QtTest>

class SimpleTest2: public QObject
{
	Q_OBJECT
private slots:
	void testOne();
};

void SimpleTest2::testOne()
{
	QString str = "Hello";
	QVERIFY(str.toUpper() == "HELLO");
	QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(SimpleTest2)
#include "simpletest2.moc"
