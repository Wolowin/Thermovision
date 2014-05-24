#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/shared_ptr.hpp"

class dupa
{
public:
	virtual void PenUp() = 0;
};

class mock_dupa: public dupa
{
public:
	MOCK_METHOD0(PenUp, void());
};

TEST(IsPrimeTest, Trivial) {
	boost::shared_ptr<dupa> qweqwewqewqe;
	mock_dupa qwe;
	dupa *ptr_Dupa;
	ptr_Dupa = & qwe;
	EXPECT_CALL (qwe, PenUp())
			.Times(1);
ptr_Dupa->PenUp();
  EXPECT_FALSE(0);
}
