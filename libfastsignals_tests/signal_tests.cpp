#include "stdafx.h"

#include "catch2/catch.hpp"
#include "libfastsignals/signal.h"

using namespace is::signals;

TEST_CASE("Can connect a few slots and emit", "[signal]") {
	signal<void(int)> valueChanged;

	int value1 = 0;
	int value2 = 0;
	valueChanged.connect([&value1](int value) {
		value1 = value;
	});
	valueChanged.connect([&value2](int value) {
		value2 = value;
	});
	REQUIRE(value1 == 0);
	REQUIRE(value2 == 0);

	valueChanged(10);
	REQUIRE(value1 == 10);
	REQUIRE(value2 == 10);
}

TEST_CASE("Can disconnect slot with explicit call", "[signal]") {
	signal<void(int)> valueChanged;

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	auto conn1 = valueChanged.connect([&value1](int value) {
		value1 = value;
	});
	auto conn2 = valueChanged.connect([&value2](int value) {
		value2 = value;
	});
	valueChanged.connect([&value3](int value) {
		value3 = value;
	});
	REQUIRE(value1 == 0);
	REQUIRE(value2 == 0);
	REQUIRE(value3 == 0);

	valueChanged(10);
	REQUIRE(value1 == 10);
	REQUIRE(value2 == 10);
	REQUIRE(value3 == 10);

	conn2.disconnect();
	valueChanged(-99);
	REQUIRE(value1 == -99);
	REQUIRE(value2 == 10);
	REQUIRE(value3 == -99);

	conn1.disconnect();
	valueChanged(17);
	REQUIRE(value1 == -99);
	REQUIRE(value2 == 10);
	REQUIRE(value3 == 17);
}

TEST_CASE("Can disconnect slot with scoped_connection", "[signal]") {
	signal<void(int)> valueChanged;

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	{
		scoped_connection conn1 = valueChanged.connect([&value1](int value) {
			value1 = value;
		});
		{
			scoped_connection conn2 = valueChanged.connect([&value2](int value) {
				value2 = value;
			});
			valueChanged.connect([&value3](int value) {
				value3 = value;
			});
			REQUIRE(value1 == 0);
			REQUIRE(value2 == 0);
			REQUIRE(value3 == 0);

			valueChanged(10);
			REQUIRE(value1 == 10);
			REQUIRE(value2 == 10);
			REQUIRE(value3 == 10);
		}

		// conn2 disconnected.
		valueChanged(-99);
		REQUIRE(value1 == -99);
		REQUIRE(value2 == 10);
		REQUIRE(value3 == -99);
	}

	// conn1 disconnected.
	valueChanged(17);
	REQUIRE(value1 == -99);
	REQUIRE(value2 == 10);
	REQUIRE(value3 == 17);
}

TEST_CASE("Can disconnect all", "[signal]") {
	signal<void(int)> valueChanged;

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
	valueChanged.connect([&value1](int value) {
		value1 = value;
	});
	valueChanged.connect([&value2](int value) {
		value2 = value;
	});
	valueChanged.connect([&value3](int value) {
		value3 = value;
	});
	REQUIRE(value1 == 0);
	REQUIRE(value2 == 0);
	REQUIRE(value3 == 0);

	valueChanged(63);
	REQUIRE(value1 == 63);
	REQUIRE(value2 == 63);
	REQUIRE(value3 == 63);

	valueChanged.disconnect_all();
	valueChanged(101);
	REQUIRE(value1 == 63);
	REQUIRE(value2 == 63);
	REQUIRE(value3 == 63);
}
