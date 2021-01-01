#include <mgfutils.h>

float angleNormal(float a) {
  if(a < 0.f)
    return a + M_PI * 2.f;
  if(a >= M_PI * 2.f)
    return a - M_PI * 2.f;

	return a;
}

float floatApproach(float val, float f1, float f2) {
	float tmp = f1 - f2;
	if(tmp > 0) {
    if(tmp < val)
      return f2;
    else
      return f1 - val;
	} else if(tmp < 0) {
    tmp = -tmp;
    if(tmp < val)
      return f2;
    else
      return f1 + val;
	} else
    return f2;
}

float angleApproach(float val, float a1, float a2) {
  float tmp = a1 - a2;
	if(tmp > 0) {
		if(tmp < val) {
      return angleNormal(a2);
		} else {
			if(tmp < M_PI) {
        return angleNormal(a1 - val);
			} else {
        return angleNormal(a1 + val);
			}
		}
	} else if(tmp < 0) {
    tmp = -tmp;
		if(tmp < val) {
      return angleNormal(a2);
		} else {
			if(tmp < M_PI) {
        return angleNormal(a1 + val);
			} else {
        return angleNormal(a1 - val);
			}
		}
	} else
    return angleNormal(a2);
}

/*float d2r(float angle) {
	return angle / 180.f * (float)M_PI;
}*/
