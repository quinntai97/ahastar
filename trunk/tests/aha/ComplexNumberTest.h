/*
 *  ComplexNumberTest.h
 *  hog
 *
 *  Created by Daniel Harabor on 3/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;

class Complex { 
  friend bool operator ==(const Complex& a, const Complex& b) {   return a.real == b.real  &&  a.imaginary == b.imaginary; }
  double real, imaginary;
public:
  Complex( double r, double i = 0 ) 
    : real(r)
        , imaginary(i) 
  {
  }
};
class ComplexNumberTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE (ComplexNumberTest);
    CPPUNIT_TEST (testEquality);
    CPPUNIT_TEST_SUITE_END ();

private:
  Complex *m_10_1, *m_1_1, *m_11_2;
public:
  void setUp();
  void tearDown();
  
  void testEquality();
/*  void testAddition()
  {
    //CPPUNIT_ASSERT( *m_10_1 + *m_1_1 == *m_11_2 );
  }*/
};

CPPUNIT_TEST_SUITE_REGISTRATION (ComplexNumberTest);
