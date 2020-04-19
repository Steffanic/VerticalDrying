#include <iostream>
#include <vector>
#include "Intersect.h"
#include <stdio.h>
#include <string.h>
#include "psrand.h"
#include <chrono>
#include <fstream>
#include <math.h>
#include <bits/stdc++.h> 
#include <algorithm>

#define PI 3.14159265
#define MAXINT 10254485504*100/55
using namespace std; 
using namespace std::chrono;

const double length = 0.5;
const double density = 0.01;
const double L = 100;
const double Hi = 100;
double H = Hi;
const int n_sticks = density*L*Hi/length;
const double dl = 0.1;
const double dth = 2*dl;
const double mu = 0.0001;//.0000537;
double step = 0;

struct Line sticks[100000];

void evaporate()
{
	double oldH=H;
	H = Hi*exp(-mu*step);
	double diff=oldH-H;
	cout<<"\rEvaporating!";
	//cout<<"H="<<H<<endl;
	/*for(int i=0;i<n_sticks;i++)
	{
		if(enforceHeightBoundary(sticks[i]))
		{
			sticks[i].a.y = sticks[i].a.y - diff;
			sticks[i].a.y = sticks[i].a.y - diff;
		}
	}*/
}

int checkIntersectAny(struct Line stick)
{//returns true if there are any intersections
	for(int i=0;i<n_sticks;i++)
	{
		if(i==stick.id)
		{
			continue;
		}
		if(doIntersect(stick, sticks[i]))
		{
			return sticks[i].id;
		}
	}
	return -1;
}

bool enforceHeightBoundary(struct Line stick)
{
	//cout<<"Height enforcement: H="<<H<<" a.y="<<stick.a.y<<" b.y="<<stick.b.y<<endl;
	if(stick.a.y>H||stick.b.y>H)
	{
		return true;
	}
	return false;
}

bool enforceBoundary(struct Line stick)
{
	//cout<<"Height enforcement: H="<<H<<" a.y="<<stick.a.y<<" b.y="<<stick.b.y<<endl;
	if(stick.a.y<0||stick.b.y<0||stick.a.x<0||stick.a.x>L||stick.b.x<0||stick.b.x>L)
	{
		return true;
	}
	return false;
}

bool throwSticks(int n_sticks, struct psRand rand)
{
	cout<<"Throwing sticks!"<<endl;
	double x1;
	double x2;
	double y1;
	double y2;

	for(int k=0; k<n_sticks; k++)
	{
		double cx = fmod((double)rand.getRando()/(double)MAXINT, L-2);
		double cy = fmod((double)rand.getRando()/(double)MAXINT, Hi-2);
	
		double th = fmod((double)rand.getRando()*20/(double)MAXINT, 180)*PI/180;
		double ct = cos(th);
		double st = sin(th);


		x1 = min(cx-length/2*ct, cx+length/2*ct);
		x2 = max(cx-length/2*ct, cx+length/2*ct);
		if(th>90*PI/180)
		{
			y1 = cy+length/2*st;
			y2 = cy-length/2*st;
		}else{
			y1 = cy-length/2*st;
			y2 = cy+length/2*st;
		}

		

		struct Line cand = {x1, y1, x2, y2, k};
		//cout<<"Any Intersections? "<<checkIntersectAny(cand)<<endl;
		if(checkIntersectAny(cand)!=-1||enforceHeightBoundary(cand)||enforceBoundary(cand))
		{
			//cout<<"We have an intersection with: "<<checkIntersectAny(cand)<<"At theta="<<th<<endl;
			k--;
		}else{
			sticks[k] = {x1,y1,x2,y2, k};
		}

	}
}

bool brownianTranslation(struct Line stick, struct psRand rand)
{
	double ogx1 = stick.a.x;
	double ogx2 = stick.b.x;
	double ogy1 = stick.a.y;
	double ogy2 = stick.b.y;
	//cout<<ogx1<<endl;
	do{
		double th = fmod((double)rand.getRando()*20/(double)MAXINT, 180)*PI/180;
		stick.a.x = ogx1+dl*cos(th);
		stick.b.x = ogx2+dl*cos(th);
		stick.a.y = ogy1+dl*sin(th);
		stick.b.y = ogy2+dl*sin(th);
		//cout<<stick<<endl;

	}
	while(checkIntersectAny(stick)||enforceHeightBoundary(stick));
}

bool brownianTranslateandRotate(int cid, struct psRand rand)
{
	
	double ogx1 = sticks[cid].a.x;
	double ogx2 = sticks[cid].b.x;
	double ogy1 = sticks[cid].a.y;
	double ogy2 = sticks[cid].b.y;
	do{
		double th1 = fmod((double)rand.getRando()*20/(double)MAXINT, 360)*PI/180;
		//cout<<"ogy1 "<<ogy1<<"th "<<th1<<"cose(th1)"<<cos(th1)<<endl;
		sticks[cid].a.x = ogx1+dl*cos(th1);
		sticks[cid].b.x = ogx2+dl*cos(th1);
		sticks[cid].a.y = ogy1+dl*sin(th1);
		sticks[cid].b.y = ogy2+dl*sin(th1);

		int dir = rand.getRando()%100>50?1:-1;
		double th = atan((sticks[cid].b.y-sticks[cid].a.y)/(sticks[cid].b.x-sticks[cid].a.x))+dir*dth;
		double ct = cos(th);
		double st = sin(th);
		double cx = (sticks[cid].a.x+sticks[cid].b.x)/2;
		double cy = (sticks[cid].a.y+sticks[cid].b.y)/2;
		sticks[cid].a.x = min(cx-length/2*ct, cx+length/2*ct);
		sticks[cid].b.x = max(cx-length/2*ct, cx+length/2*ct);
		if(th>90*PI/180)
		{
			sticks[cid].a.y = cy+length/2*st;
			sticks[cid].b.y = cy-length/2*st;
		}else{
			sticks[cid].a.y = cy-length/2*st;
			sticks[cid].b.y = cy+length/2*st;
		}
		//cout<<"Fail check: Stick "<<sticks[cid]<<" OG x1 "<<ogx1<<" Intersection "<<checkIntersectAny(sticks[cid])<<" Height "<<enforceHeightBoundary(sticks[cid])<<" Boundary "<<enforceBoundary(sticks[cid])<<endl;


	}
	while(checkIntersectAny(sticks[cid])!=-1||enforceHeightBoundary(sticks[cid])||enforceBoundary(sticks[cid]));
	
}

int main()
{
	cout<<"Starting"<<endl;
	psRand rand(1L << 11);

	ofstream file;
	file.open("dat_files/stick_00.dat");
	throwSticks(n_sticks, rand);
	file<<sticks[0];
	for(int k=1;k<n_sticks;k++)
	{
		file<<sticks[k];
	}
	file.close();
	char buf[30];
	for(int i=1;i<1000;i++)
	{
		step=i;
		evaporate();
		cout<<"\rRunning step "<<i<<" with a translate and a rotate for "<<n_sticks<<" random sticks. Height is "<<H<<flush;
		sprintf(buf, "dat_files/stick_0%d.dat", i);
		file.open(buf);
		for(int j=0;j<n_sticks;j++)
		{
			int cid = rand.getRando()%n_sticks;
			brownianTranslateandRotate(cid, rand);
		}
		file<<sticks[0];
		for(int k=1;k<n_sticks;k++)
		{
			file<<sticks[k];
		}
		file.close();
	}
}