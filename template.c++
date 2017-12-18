#include<iostream>
#include<vector>
#define SIZE 5

using namespace std;

struct image{
	int im[SIZE*SIZE];
	int get_value(int x, int y){
		return im[x*SIZE+y];
	}
	void set_value(int x, int y, int value){
		im[x*SIZE+y] = value;
	}
	int size;
};

struct pixel{
	pixel(int x_v, int y_v, int s_v){
		x = x_v;
		y = y_v;
		scale = s_v;
	}
	int x;
	int y;
	int scale;
};

int error(image* target, image* template_image, int x, int y){
	int size = template_image->size;
	int error = 0;
	for(int i=x; i<x+size; ++i){
		for(int j=y; j<y+size; ++j){
			if (target->get_value(i, j)-template_image->get_value(i-x,j-y) != 0)
				return 1;
		}
	}
	return 0;
}

void compare(image* target, image* template_image, vector<pixel>* result){
	int size_small = template_image->size, size_large = target->size, end = size_large-size_small;
	for(int i=0; i<=end; ++i){
		for(int j=0; j<=end; ++j){
			if(error(target, template_image, i, j) == 0){
				result->push_back(pixel(i, j, template_image->size));
				cout<<size_small<<": "<<i<<", "<<j<<endl;
			}
		}
	}
}

void downsample(image* template_image){
	template_image->size = template_image->size/2;
	if(template_image->size==0){
		return;
	}
	else{
		int output_size = template_image->size;
		for(int i=0; i<output_size; ++i){
			for(int j=0; j<output_size; ++j){
				int temp = template_image->get_value(i*2, j*2);
				template_image->set_value(i,j,temp); 
			}
		}
	}
}

void output(vector<pixel>* result){
	cout<<result->size()<<endl;
	int ss = result->size();
	for (vector<pixel>::reverse_iterator iter = result->rbegin(); iter != result->rend(); iter++){
		cout<<iter->scale<<": "<<iter->x<<", "<<iter->y;
		if (iter+1 != result->rend())
			cout<<endl;
	}

}

int main(){
	image target, template_image;
	vector<pixel> result;
	int target_size, template_size;
	// cout<<"input target_size" << endl;
	cin>>target_size;
	target.size = target_size;
	for(int i=0; i<target_size; ++i){
		for(int j=0; j<target_size; ++j){
			cin>>target.im[i*SIZE+j];
			if(!(i==target_size-1&&j==target_size-1)){
				char temp;
				cin>>temp;
			}
		}
	}
	// cout<<"input template_size" << endl;
	cin>>template_size;
	template_image.size = template_size;
	for(int i=0; i<template_size; ++i){
		for(int j=0; j<template_size; ++j){
			cin>>template_image.im[i*SIZE+j];
			if(!(i==template_size-1&&j==template_size-1)){
				char temp;
				cin>>temp;
			}
		}
	}
	while(template_image.size >= 1){
		compare(&target, &template_image, &result);
		downsample(&template_image);
	}
	// output(&result);
	return 0;
}