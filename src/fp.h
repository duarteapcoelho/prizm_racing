#pragma once
#include <stdio.h>
#include <stdlib.h>

#define FP_RIGHT 1024
#define FP_RIGHT_BITS 10
class fp {
	public:
	int i;
	inline fp(int v){
		i = v << FP_RIGHT_BITS;
	}
	inline fp(float v){
		i = v * FP_RIGHT;
	}
	inline fp(double v){
		i = v * FP_RIGHT;
	}
	inline fp(){ i = 0; }

	inline operator int() { return i >> FP_RIGHT_BITS; }
	inline operator float() { return ((float)i) / ((float)FP_RIGHT); }

	inline fp operator + (fp o){
		fp r;
		r.i = i + o.i;
		return r;
	}
	inline fp operator - (fp o){
		fp r;
		r.i = i - o.i;
		return r;
	}
	inline fp operator * (fp o){
		// printf("%d %d %d\n", i, o.i, INT_MAX);
		fp r;
		r.i = (i * o.i) >> FP_RIGHT_BITS;
		return r;
	}
	inline fp operator / (fp o){
		fp r;
		r.i = (i<<FP_RIGHT_BITS) / o.i;
		return r;
	}

	// fp& operator += (const fp& o){
	// 	i += o.i;
	// 	return *this;
	// }
	// fp& operator -= (const fp& o){
	// 	i += o.i;
	// 	return *this;
	// }
	// fp& operator *= (const fp& o){
	// 	i += o.i;
	// 	return *this;
	// }
	// fp& operator /= (const fp& o){
	// 	i += o.i;
	// 	return *this;
	// }

	inline bool operator > (fp o){ return i > o.i; }
	inline bool operator < (fp o){ return i < o.i; }
	inline bool operator > (int o){ return i > (o<<FP_RIGHT_BITS); }
	inline bool operator < (int o){ return i < (o<<FP_RIGHT_BITS); }
	inline bool operator > (float o){ return i > o*FP_RIGHT; }
	inline bool operator < (float o){ return i < o*FP_RIGHT; }
	inline bool operator > (double o){ return i > o*FP_RIGHT; }
	inline bool operator < (double o){ return i < o*FP_RIGHT; }
	inline bool operator >= (fp o){ return i >= o.i; }
	inline bool operator <= (fp o){ return i <= o.i; }
	inline bool operator >= (int o){ return i >= (o<<FP_RIGHT_BITS); }
	inline bool operator <= (int o){ return i <= (o<<FP_RIGHT_BITS); }
	inline bool operator >= (float o){ return i >= o*FP_RIGHT; }
	inline bool operator <= (float o){ return i <= o*FP_RIGHT; }
	inline bool operator >= (double o){ return i >= o*FP_RIGHT; }
	inline bool operator <= (double o){ return i <= o*FP_RIGHT; }

	inline bool operator == (fp o){ return i == o.i; }
	inline bool operator == (int o){ return i == (o<<FP_RIGHT_BITS); }
	inline bool operator == (float o){ return i == o*FP_RIGHT; }
	inline bool operator == (double o){ return i == o*FP_RIGHT; }
};

// typedef float fp;
