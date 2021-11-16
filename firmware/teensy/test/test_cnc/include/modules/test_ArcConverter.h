#ifndef TEST_CNC_MODULES_ARC_CONVERTER_H
#define TEST_CNC_MODULES_ARC_CONVERTER_H

void test_getCenterPointFromRadius();

void test_ArcConverter_invalidCode();
void test_ArcConverter_invalidCenterPoint();
void test_ArcConverter_invalidRadius();
void test_ArcConverter_invalidP();
void test_ArcConverter_absoluteR();
void test_ArcConverter_g2XY();
void test_ArcConverter_g3XY();
void test_ArcConverter_helixXY();
void test_ArcConverter_fullXY();
void test_ArcConverter_rXY();
void test_ArcConverter_p2XY();
void test_ArcConverter_offsetAndAbsolute();
void test_ArcConverter_g2XZ();
void test_ArcConverter_g3XZ();
void test_ArcConverter_g2YZ();
void test_ArcConverter_g3YZ();

#endif
