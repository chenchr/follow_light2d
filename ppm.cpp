#include<fstream>
#include<sstream>
#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<limits>

#define EPISLON 1e-6f
#define LIGHT_VAL 2
#define TWO_PI 6.28318530718f
#define MAX_STEP 64
#define MAX_DIST 2.0f
#define IM_SIZE 512

using namespace std;

void write_image(ofstream& stream, int width, int height, unsigned char* data){
	int all = height*width*3;
	stringstream ss;
	ss.str("");
	ss<<height;
	stream<<"P6"<<' '<<ss.str()<<' ';
	ss.str("");
	ss<<width;
	stream<<ss.str()<<' ';
	stream<<255<<" ";
	stream.write((char*)data, all);
}

struct result{
	float sdf;
	float emissive;
};

float circleSdf(float x, float y, float ox, float oy, float radius){
	float ux = x-ox, uy = y-oy;
	return sqrtf(ux*ux + uy*uy)-radius;
}

float planeSdf(float x, float y, float px, float py, float nx, float ny){
	return (x-px)*nx+(y-py)*ny;
}

float segmentSdf(float x, float y, float ax, float ay, float bx, float by){
	float vx = x-ax, vy = y-ay;
	float ux = bx-ax, uy = by-ay;
	float maxmin = fmaxf(fminf((vx*ux+vy*uy)/(ux*ux+uy*uy), 1.0f), 0.0f);
	float x_x = ax+maxmin*ux, x_y = ay+maxmin*uy;
	float delta_x = x-x_x, delta_y = y-x_y;
	return sqrtf(delta_x*delta_x+delta_y*delta_y);
}

float capsuleSdf(float x, float y, float ax, float ay, float bx, float by, float radius){
	return segmentSdf(x, y, ax, ay, bx, by) - radius;
}

float boxSdf(float x, float y, float cx, float cy, float theta, float sx, float sy){
	float cos_theta = cosf(theta), sin_theta = sinf(theta);
	float xsc_x = x-cx, xsc_y = y-cy;
	float x_x = fabsf(cos_theta*xsc_x + sin_theta*xsc_y) - sx;
	float x_y = fabsf(-sin_theta*xsc_x + cos_theta*xsc_y) - sy;
	float ax = fmaxf(x_x, 0.0f), ay = fmaxf(x_y, 0.0f);
	return fminf(fmaxf(x_x, x_y), 0.0f) + sqrtf(ax*ax+ay*ay);
}

float roundedBoxSdf(float x, float y, float cx, float cy, float theta, float sx, float sy, float radius){
	return boxSdf(x,y,cx,cy,theta,sx,sy)-radius;
}

float triangleSdf(float x, float y, float ax, float ay, float bx, float by, float cx, float cy){
	float d = fminf(fminf(
		segmentSdf(x,y,ax,ay,bx,by), 
		segmentSdf(x,y,bx,by,cx,cy)),
		segmentSdf(x,y,cx,cy,ax,ay));
	return (bx - ax) * (y - ay) > (by - ay) * (x - ax) && 
           (cx - bx) * (y - by) > (cy - by) * (x - bx) && 
           (ax - cx) * (y - cy) > (ay - cy) * (x - cx) ? -d : d;
}

float roundedTriangleSdf(float x,float y,float ax,float ay,float bx,float by,float cx,float cy,float radius){
	return triangleSdf(x,y,ax,ay,bx,by,cx,cy)-radius;
}

result unionOp(result left, result right){
	return left.sdf<right.sdf?left:right;
}

result intersectOp(result left, result right){
	result temp = left.sdf > right.sdf ? left:right;
	temp.sdf = left.sdf > right.sdf ? left.sdf:right.sdf;
	return temp;
}

result substractOp(result left, result right){
	result temp = left;
	temp.sdf = left.sdf>-right.sdf?left.sdf:-right.sdf;
	return temp;
}

result scene(float x, float y){
	// result r = {circleSdf(x, y, 0.5, 0.5, 0.2), 2};
	// result r1 = { circleSdf(x, y, 0.3f, 0.3f, 0.10f), 2.0f };
 //    result r2 = { circleSdf(x, y, 0.3f, 0.7f, 0.05f), 0.8f };
 //    result r3 = { circleSdf(x, y, 0.7f, 0.5f, 0.10f), 0.0f };

 //    return unionOp(unionOp(r1, r2), r3);
	// result a = { circleSdf(x, y, 0.4f, 0.5f, 0.20f), 1.0f };
 //    result b = { circleSdf(x, y, 0.6f, 0.5f, 0.20f), 0.8f };
 //    return substractOp(a, b);
	// return r;
	// result r1 = { circleSdf(x, y, 0.5f, 0.5f, 0.2f), 1.0f };
	// result r2 = {planeSdf(x, y, 0.0f, 0.5f, 0.0f, 1.0f), 0.8f};
	// return intersectOp(r1, r2);
	// result r = {capsuleSdf(x,y,0.4f,0.4f,0.6f,0.6f,0.1f), 1.0f};
	// return r;
	// result r = {boxSdf(x, y, 0.5f, 0.5f, TWO_PI/16.0f, 0.3f, 0.1f), 1.0f};
	// result r = {roundedBoxSdf(x, y, 0.5f, 0.5f, TWO_PI/16.0f, 0.3f, 0.1f, 0.1f), 1.0f};
	// return r;
	// result r = {triangleSdf(x,y,0.5f,0.2f,0.8f,0.8f,0.3f,0.6f), 1.0f};
	// return r;
	// result r = {roundedTriangleSdf(x,y,0.5f,0.2f,0.8f,0.8f,0.3f,0.6f,0.1f), 1.0f};
	// return r;
	result a = {circleSdf(x,y,0.4f,0.2f,0.1f),2.0f};
	result b = {boxSdf(x,y,0.5f,0.8f,TWO_PI/16.0f,0.1f,0.1f), 0.0f};
	result c = {boxSdf(x,y,0.8f,0.5f,TWO_PI/16.0f,0.1f,0.1f), 0.0f};
	return unionOp(unionOp(a, b), c);
}

void gradient(float x, float y, float* nx, float* ny){
	*nx = (scene(x + EPISLON, y).sdf - scene(x - EPISLON, y).sdf)*(0.5f/EPISLON);
	*ny = (scene(x, y + EPISLON).sdf - scene(x, y - EPISLON).sdf)*(0.5f/EPISLON);
}

float light(float x, float y, float dx, float dy){
	int step_num=0;
	float dist=0.001f;
	while(step_num<MAX_STEP&&dist<MAX_DIST){
		result d = scene(x+dist*dx, y+dist*dy);
		if(d.sdf<EPISLON)
			return d.emissive;
		dist += d.sdf;
		++step_num;
	}
	return 0.0;
}

float traversal(float x, float y){
	float all = 0;
	int sample_num = 64;
	for(int i=0; i<sample_num; ++i){
		// float angle = TWO_PI*rand()/RAND_MAX;
		// float angle = TWO_PI*i/sample_num;
		float angle = TWO_PI * (i + (float)rand() / RAND_MAX) / sample_num;
		all += light(x, y, cosf(angle), sinf(angle));
	}
	return all/sample_num;
}


void draw_light(unsigned char* data, int height, int width){
	float light_value = 0.0;
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			// light_value = traversal((float)j/width, (float)i/height);
			float nx, ny;
			gradient((float)j/width,(float)i/height, &nx, &ny);
			unsigned char* ptr = data+i*width*3+j*3;
			ptr[0] = (int)((fmaxf(fminf(nx, 1.0f), -1.0f)*0.5f+0.5f)*255.0f);
			ptr[1] = (int)((fmaxf(fminf(ny, 1.0f), -1.0f)*0.5f+0.5f)*255.0f);
			ptr[2] = 0;
			// for(int k=0; k<3; ++k){
			// 	data[+k] = (int)fminf((light_value*255.0f),255.0f);
			// }
		}
	}
}

int main(){
	ofstream out;
	unsigned char data[IM_SIZE*IM_SIZE*3];
	// for(int i=0; i<256; ++i){
	// 	for(int j=0; j<256; ++j){
	// 		int index = i*256*3+j*3;
	// 		data[index] = (unsigned char)j;
	// 		data[index+1] = (unsigned char)i;
	// 		data[index+2] = (unsigned char)128;
	// 	}
	// }
	draw_light(data, IM_SIZE, IM_SIZE);
	out.open("test_grad.ppm", ios::out|ios::binary);
	write_image(out, IM_SIZE, IM_SIZE, data);
	out.close();
	return 0;

}