#pragma once
#include <stdio.h>
#include <stdlib.h>

template <typename T, int P, int M>
class _fp {
	public:
	T i;
	template <typename T2, int P2, int M2>
	inline _fp(_fp<T2, P2, M2> v){
		int n = P - P2;
		if(n > 0)
			i = v.i << n;
		else
			i = v.i >> (-n);
	}
	inline _fp(int v){
		i = v << P;
	}
	inline _fp(float v){
		i = v * M;
	}
	inline _fp(double v){
		i = v * M;
	}
	inline _fp(){ i = 0; }

	inline operator int() { return i >> P; }
	inline operator float() { return ((float)i) / ((float)M); }

	inline _fp operator + (_fp<T,P,M> o){
		_fp r;
		r.i = i + o.i;
		return r;
	}
	inline _fp operator - (_fp<T,P,M> o){
		_fp r;
		r.i = i - o.i;
		return r;
	}
	inline _fp operator * (_fp<T,P,M> o){
		_fp r;
		r.i = (i * o.i) >> P;
		return r;
	}
	inline _fp operator / (_fp<T,P,M> o){
		_fp r;
		r.i = (i<<P) / o.i;
		return r;
	}
	inline _fp operator % (_fp<T,P,M> o){
		_fp r;
		r.i = i % o.i;
		return r;
	}

	inline bool operator > (_fp<T,P,M> o){ return i > o.i; }
	inline bool operator < (_fp<T,P,M> o){ return i < o.i; }
	inline bool operator > (int o){ return i > (o<<P); }
	inline bool operator < (int o){ return i < (o<<P); }
	inline bool operator > (float o){ return i > o*M; }
	inline bool operator < (float o){ return i < o*M; }
	inline bool operator > (double o){ return i > o*M; }
	inline bool operator < (double o){ return i < o*M; }
	inline bool operator >= (_fp<T,P,M> o){ return i >= o.i; }
	inline bool operator <= (_fp<T,P,M> o){ return i <= o.i; }
	inline bool operator >= (int o){ return i >= (o<<P); }
	inline bool operator <= (int o){ return i <= (o<<P); }
	inline bool operator >= (float o){ return i >= o*M; }
	inline bool operator <= (float o){ return i <= o*M; }
	inline bool operator >= (double o){ return i >= o*M; }
	inline bool operator <= (double o){ return i <= o*M; }

	inline bool operator == (_fp<T,P,M> o){ return i == o.i; }
	inline bool operator == (int o){ return i == (o<<P); }
	inline bool operator == (float o){ return i == o*M; }
	inline bool operator == (double o){ return i == o*M; }
	inline bool operator != (_fp<T,P,M> o){ return i != o.i; }
	inline bool operator != (int o){ return i != (o<<P); }
	inline bool operator != (float o){ return i != o*M; }
	inline bool operator != (double o){ return i != o*M; }
};

typedef _fp<int, 10, 1024> fp;
typedef _fp<long long int, 10, 1024> lfp;
