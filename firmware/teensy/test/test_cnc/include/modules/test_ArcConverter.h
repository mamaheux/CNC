#ifndef TEST_CNC_MODULES_ARC_CONVERTER_H
#define TEST_CNC_MODULES_ARC_CONVERTER_H

void test_getCenterPointFromRadius();

void test_ArcConverter_invalidCode();
void test_ArcConverter_invalidCenterPoint();
void test_ArcConverter_reallySmallRadius();
void test_ArcConverter_invalidP();
void test_ArcConverter_absoluteR();
void test_ArcConverter_g2XY();
void test_ArcConverter_g3XY();
void test_ArcConverter_helixXY();
void test_ArcConverter_fullXY();
void test_ArcConverter_rXY();
void test_ArcConverter_p2XY();
void test_ArcConverter_offsetAndAbsolute();
void test_ArcConverter_g53();
void test_ArcConverter_g2XZ();
void test_ArcConverter_g3XZ();
void test_ArcConverter_g2YZ();
void test_ArcConverter_g3YZ();
void test_ArcConverter_g2Offset();
void test_ArcConverter_g2Rotation();
void test_ArcConverter_g2OffsetRotation();

void test_ArcConverter_moveBack();
void test_ArcConverter_clear();

#endif
