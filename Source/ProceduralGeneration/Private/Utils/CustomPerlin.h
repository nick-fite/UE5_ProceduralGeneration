#pragma once
//the purpose of this project is education, this is know purely my code.
//Removed quite a bit of code from this to get what I need and nothing more.
// https://github.com/Auburn/FastNoise
namespace CustomPerlin
{
	constexpr float Gradient[] = {
    0.130526192220052f, 0.99144486137381f, 0.38268343236509f, 0.923879532511287f, 0.608761429008721f, 0.793353340291235f, 0.793353340291235f, 0.608761429008721f,
    0.923879532511287f, 0.38268343236509f, 0.99144486137381f, 0.130526192220051f, 0.99144486137381f, -0.130526192220051f, 0.923879532511287f, -0.38268343236509f,
    0.793353340291235f, -0.60876142900872f, 0.608761429008721f, -0.793353340291235f, 0.38268343236509f, -0.923879532511287f, 0.130526192220052f, -0.99144486137381f,
    -0.130526192220052f, -0.99144486137381f, -0.38268343236509f, -0.923879532511287f, -0.608761429008721f, -0.793353340291235f, -0.793353340291235f, -0.608761429008721f,
    -0.923879532511287f, -0.38268343236509f, -0.99144486137381f, -0.130526192220052f, -0.99144486137381f, 0.130526192220051f, -0.923879532511287f, 0.38268343236509f,
    -0.793353340291235f, 0.608761429008721f, -0.608761429008721f, 0.793353340291235f, -0.38268343236509f, 0.923879532511287f, -0.130526192220052f, 0.99144486137381f,
    0.130526192220052f, 0.99144486137381f, 0.38268343236509f, 0.923879532511287f, 0.608761429008721f, 0.793353340291235f, 0.793353340291235f, 0.608761429008721f,
    0.923879532511287f, 0.38268343236509f, 0.99144486137381f, 0.130526192220051f, 0.99144486137381f, -0.130526192220051f, 0.923879532511287f, -0.38268343236509f,
    0.793353340291235f, -0.60876142900872f, 0.608761429008721f, -0.793353340291235f, 0.38268343236509f, -0.923879532511287f, 0.130526192220052f, -0.99144486137381f,
    -0.130526192220052f, -0.99144486137381f, -0.38268343236509f, -0.923879532511287f, -0.608761429008721f, -0.793353340291235f, -0.793353340291235f, -0.608761429008721f,
    -0.923879532511287f, -0.38268343236509f, -0.99144486137381f, -0.130526192220052f, -0.99144486137381f, 0.130526192220051f, -0.923879532511287f, 0.38268343236509f,
    -0.793353340291235f, 0.608761429008721f, -0.608761429008721f, 0.793353340291235f, -0.38268343236509f, 0.923879532511287f, -0.130526192220052f, 0.99144486137381f,
    0.130526192220052f, 0.99144486137381f, 0.38268343236509f, 0.923879532511287f, 0.608761429008721f, 0.793353340291235f, 0.793353340291235f, 0.608761429008721f,
    0.923879532511287f, 0.38268343236509f, 0.99144486137381f, 0.130526192220051f, 0.99144486137381f, -0.130526192220051f, 0.923879532511287f, -0.38268343236509f,
    0.793353340291235f, -0.60876142900872f, 0.608761429008721f, -0.793353340291235f, 0.38268343236509f, -0.923879532511287f, 0.130526192220052f, -0.99144486137381f,
    -0.130526192220052f, -0.99144486137381f, -0.38268343236509f, -0.923879532511287f, -0.608761429008721f, -0.793353340291235f, -0.793353340291235f, -0.608761429008721f,
    -0.923879532511287f, -0.38268343236509f, -0.99144486137381f, -0.130526192220052f, -0.99144486137381f, 0.130526192220051f, -0.923879532511287f, 0.38268343236509f,
    -0.793353340291235f, 0.608761429008721f, -0.608761429008721f, 0.793353340291235f, -0.38268343236509f, 0.923879532511287f, -0.130526192220052f, 0.99144486137381f,
    0.130526192220052f, 0.99144486137381f, 0.38268343236509f, 0.923879532511287f, 0.608761429008721f, 0.793353340291235f, 0.793353340291235f, 0.608761429008721f,
    0.923879532511287f, 0.38268343236509f, 0.99144486137381f, 0.130526192220051f, 0.99144486137381f, -0.130526192220051f, 0.923879532511287f, -0.38268343236509f,
    0.793353340291235f, -0.60876142900872f, 0.608761429008721f, -0.793353340291235f, 0.38268343236509f, -0.923879532511287f, 0.130526192220052f, -0.99144486137381f,
    -0.130526192220052f, -0.99144486137381f, -0.38268343236509f, -0.923879532511287f, -0.608761429008721f, -0.793353340291235f, -0.793353340291235f, -0.608761429008721f,
    -0.923879532511287f, -0.38268343236509f, -0.99144486137381f, -0.130526192220052f, -0.99144486137381f, 0.130526192220051f, -0.923879532511287f, 0.38268343236509f,
    -0.793353340291235f, 0.608761429008721f, -0.608761429008721f, 0.793353340291235f, -0.38268343236509f, 0.923879532511287f, -0.130526192220052f, 0.99144486137381f,
    0.130526192220052f, 0.99144486137381f, 0.38268343236509f, 0.923879532511287f, 0.608761429008721f, 0.793353340291235f, 0.793353340291235f, 0.608761429008721f,
    0.923879532511287f, 0.38268343236509f, 0.99144486137381f, 0.130526192220051f, 0.99144486137381f, -0.130526192220051f, 0.923879532511287f, -0.38268343236509f,
    0.793353340291235f, -0.60876142900872f, 0.608761429008721f, -0.793353340291235f, 0.38268343236509f, -0.923879532511287f, 0.130526192220052f, -0.99144486137381f,
    -0.130526192220052f, -0.99144486137381f, -0.38268343236509f, -0.923879532511287f, -0.608761429008721f, -0.793353340291235f, -0.793353340291235f, -0.608761429008721f,
    -0.923879532511287f, -0.38268343236509f, -0.99144486137381f, -0.130526192220052f, -0.99144486137381f, 0.130526192220051f, -0.923879532511287f, 0.38268343236509f,
    -0.793353340291235f, 0.608761429008721f, -0.608761429008721f, 0.793353340291235f, -0.38268343236509f, 0.923879532511287f, -0.130526192220052f, 0.99144486137381f,
    0.38268343236509f, 0.923879532511287f, 0.923879532511287f, 0.38268343236509f, 0.923879532511287f, -0.38268343236509f, 0.38268343236509f, -0.923879532511287f,
    -0.38268343236509f, -0.923879532511287f, -0.923879532511287f, -0.38268343236509f, -0.923879532511287f, 0.38268343236509f, -0.38268343236509f, 0.923879532511287f,

	};

	constexpr float Gradient3D[] = {
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    0, 1, 1, 0,  0,-1, 1, 0,  0, 1,-1, 0,  0,-1,-1, 0,
    1, 0, 1, 0, -1, 0, 1, 0,  1, 0,-1, 0, -1, 0,-1, 0,
    1, 1, 0, 0, -1, 1, 0, 0,  1,-1, 0, 0, -1,-1, 0, 0,
    1, 1, 0, 0,  0,-1, 1, 0, -1, 1, 0, 0,  0,-1,-1, 0
	};

	class FNoiseGenerator
	{
	public:
		//Defaults to .01
		float Frequency = .01;
		//defaults to 1337
		int BaseSeed = 1337;
		//defaults to 3
		int BaseOctaves = 3;
		//defaults to 1 / 1.75f
		float BaseFractalBounding = 1 / 1.75f;
		//defaults to 0
		float BaseWeightedStrength = 0;
		//Hashing?
	private:
		const int PrimeX = 501125321;
		const int PrimeY = 1136930381;
		const int PrimeZ = 1720413743;

		//faster (and better) then UE lerp for this simple use Case :)
		static float Lerp(float a, float b, float t) { return a + t * (b - a); }

		//Gets floor
    	static int FastFloor(float x){return x >= 0 ? x : x - 1;}

		//gets quintic
		static float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

		FORCEINLINE void TransformNoiseCoordinate(float& X, float& Y)
		{
			X *= Frequency;
			Y *= Frequency;

			const float Sqrt3 = 1.7320508075688772935274463415059;
			const float F2 = 0.5f * (Sqrt3 -1);
			const float t = (X + Y) * F2;

			X += t;
			Y += t;
		}
	

		FORCEINLINE float GradCoord(int seed, int XPrimed, int YPrimed, int ZPrimed, float Xd, float Yd, float Zd)
    	{
    		int Hash = seed ^ XPrimed ^ YPrimed ^ ZPrimed;
    		Hash ^= Hash >> 15;
    		Hash &= 63 << 2;

    		const float Xg = Gradient3D[Hash];
    		const float Yg = Gradient3D[Hash | 1];
    		const float Zg = Gradient3D[Hash | 2];

    		return Xd * Xg + Yd * Yg + Zd * Zg;
    	}

		FORCEINLINE float GradCoord(int seed, int XPrimed, int YPrimed, float Xd, float Yd)
    	{
    		int Hash = seed ^ XPrimed ^ YPrimed;
    		Hash *= 0x27d4eb2d;
    		Hash ^= Hash >> 15;
    		Hash &= 127 << 1;

    		const float Xg = Gradient[Hash];
    		const float Yg = Gradient[Hash | 1];

    		return Xd * Xg + Yd * Yg;
    	}
		
		FORCEINLINE float SinglePerlinNoise(int Seed, float X, float Y)
    	{
    		int X0 = FastFloor(X);
    		int Y0 = FastFloor(Y);
    		float Xd0 = X - X0;
    		float Yd0 = Y - Y0;
    		float Xd1 = Xd0 - 1;
    		float Yd1 = Yd0 - 1;

    		float Xs = InterpQuintic(Xd0);
    		float Ys = InterpQuintic(Yd0);

    		X0 *= PrimeX;
    		Y0 *= PrimeY;

    		int X1 = X0 + PrimeX;
    		int Y1 = Y0 + PrimeY;

    		float Xf0 = Lerp(
    			GradCoord(Seed, X0, Y0, Xd0, Yd0),
    			GradCoord(Seed, X1, Y0, Xd1, Yd0),
    			Xs);

    		float Xf1 = Lerp(
    			GradCoord(Seed, X0, Y1, Xd0, Yd1),
				GradCoord(Seed, X1, Y1, Xd1, Yd1),
				Xs);

    		return Lerp(Xf0, Xf1, Ys) * 1.4247691104677813f; //Why do I need this number??
    	}

    	
		FORCEINLINE float GenFractalFBm(float X, float Y)
		{
			int seed = BaseSeed;
			float sum = 0;
			float amp = BaseFractalBounding;

			for (int i = 0; i < BaseOctaves; i ++)
			{
				float noise = SinglePerlinNoise(seed++, X, Y);
				sum += amp * noise;
				amp *= Lerp(1.0, (noise + 1 < 2 ? noise + 1: 2) * .5f, BaseWeightedStrength);
				X *= 2;
				Y *= 2;
				amp *= .5f;
			}
    		return sum;
		}
	public:
		FORCEINLINE float GetNoise(float X, float Y)
		{
			TransformNoiseCoordinate(X, Y);
			return GenFractalFBm(X, Y);
		}
		
		FORCEINLINE void TransformNoiseCoordinate(float& X, float& Y, float& Z)
		{
			X *= Frequency;
			Y *= Frequency;
			Z *= Frequency;

		    constexpr float R3 = 2.0f/3.0f;
	 		const float R = (X + Y + Z) * R3;
	 		X = R - X;
    		Y = R - Y;
    		Z = R - Z;
		}
		
		FORCEINLINE float SinglePerlinNoise(int Seed, float X, float Y, float Z)
		{
			int X0 = FastFloor(X);
			int Y0 = FastFloor(Y);
			int Z0 = FastFloor(Z);
			
    		float Xd0 = X - X0;
    		float Yd0 = Y - Y0;
    		float Zd0 = Z - Z0;
    		float Xd1 = Xd0 - 1;
    		float Yd1 = Yd0 - 1;
    		float Zd1 = Zd0 - 1;

	    	float Xs = InterpQuintic(Xd0);
			float Ys = InterpQuintic(Yd0);
			float Zs = InterpQuintic(Zd0);

    		X0 *= PrimeX;
    		Y0 *= PrimeY;
    		Z0 *= PrimeZ;

    		int X1 = X0 + PrimeX;
    		int Y1 = Y0 + PrimeY;
    		int Z1 = Z0 + PrimeZ;

			float Xf00 = Lerp(
    			GradCoord(Seed, X0, Y0, Z0, Xd0, Yd0, Zd0),
    			GradCoord(Seed, X1, Z0, Y0, Xd1, Yd0, Zd0),
    			Xs);

    		float Xf10 = Lerp(
    			GradCoord(Seed, X0, Y1, Z0, Xd0, Yd1, Zd0),
				GradCoord(Seed, X1, Y1, Z0, Xd1, Yd1, Zd0),
				Xs);

			float Xf01 = Lerp(
    			GradCoord(Seed, X0, Y0, Z1, Xd0, Yd0, Zd1),
				GradCoord(Seed, X1, Y0, Z1, Xd1, Yd0, Zd1),
				Xs);
			float Xf11 = Lerp(
    			GradCoord(Seed, X0, Y1, Z1, Xd0, Yd1, Zd1),
				GradCoord(Seed, X1, Y1, Z1, Xd1, Yd1, Zd1),
				Xs);

			float Yf0 = Lerp(Xf00, Xf10, Ys);
			float Yf1 = Lerp(Xf01, Xf11, Ys);
			
			return Lerp(Yf0, Yf1, Zs) * 0.964921414852142333984375f;
		}
	
		FORCEINLINE float GenFractalFBm(float X, float Y, float Z)
		{
			int seed = BaseSeed;
			float sum = 0;
			float amp = BaseFractalBounding;

			for (int i = 0; i < BaseOctaves; i ++)
			{
				//float noise = SinglePerlinNoise()
				float noise = SinglePerlinNoise(seed++, X, Y, Z);
				sum += (noise * -2 + 1) * amp;
				amp *= Lerp(1.0, (noise + 1) * .5f, BaseWeightedStrength);
				X *= 2;
				Y *= 2;
				Z *= 2;
				amp *= .5f;
			}
    		return sum;
		}

		FORCEINLINE float GetNoise(float X, float Y, float Z)
		{
			TransformNoiseCoordinate(X, Y, Z);
			return GenFractalFBm(Y, Y, Z);
		}
    };
	
}