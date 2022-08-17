#pragma once
#include <stdio.h>
#include <stdlib.h>

#define FP_RIGHT 1000
class fp {
	public:
	int i;
	fp(int v){
		i = v * FP_RIGHT;
	}
	fp(float v){
		i = v * FP_RIGHT;
	}
	fp(double v){
		i = v * FP_RIGHT;
	}
	fp(){ i = 0; }

	operator int() { return i / FP_RIGHT; }
	operator float() { return ((float)i) / ((float)FP_RIGHT); }

	fp operator + (fp o){
		fp r;
		r.i = i + o.i;
		return r;
	}
	fp operator - (fp o){
		fp r;
		r.i = i - o.i;
		return r;
	}
	fp operator * (fp o){
		// printf("%d %d %d\n", i, o.i, INT_MAX);
		fp r;
		r.i = (i * o.i) / FP_RIGHT;
		return r;
	}
	fp operator / (fp o){
		fp r;
		r.i = i*FP_RIGHT / o.i;
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

	bool operator > (fp o){ return i > o.i; }
	bool operator < (fp o){ return i < o.i; }
	bool operator > (int o){ return i > o*FP_RIGHT; }
	bool operator < (int o){ return i < o*FP_RIGHT; }
	bool operator > (float o){ return i > o*FP_RIGHT; }
	bool operator < (float o){ return i < o*FP_RIGHT; }
	bool operator > (double o){ return i > o*FP_RIGHT; }
	bool operator < (double o){ return i < o*FP_RIGHT; }
	bool operator >= (fp o){ return i >= o.i; }
	bool operator <= (fp o){ return i <= o.i; }
	bool operator >= (int o){ return i >= o*FP_RIGHT; }
	bool operator <= (int o){ return i <= o*FP_RIGHT; }
	bool operator >= (float o){ return i >= o*FP_RIGHT; }
	bool operator <= (float o){ return i <= o*FP_RIGHT; }
	bool operator >= (double o){ return i >= o*FP_RIGHT; }
	bool operator <= (double o){ return i <= o*FP_RIGHT; }

	bool operator == (fp o){ return i == o.i; }
	bool operator == (int o){ return i == o*FP_RIGHT; }
	bool operator == (float o){ return i == o*FP_RIGHT; }
	bool operator == (double o){ return i == o*FP_RIGHT; }
};

// typedef float fp;
