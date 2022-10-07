#include "stdafx.h"
//#include <iostream>
#include <fstream>
#include "gnuplot.h"
#include <random>
#include <windows.h>
#include <math.h>
#define N 500 //set the number of particles

//using namespace std;

int main()
{
	Gnuplot plot; //use of external Gnuplot library
	ofstream myfile;
	myfile.open("gnutest.data"); //opens data file

	const long double PI = 3.141592653589793238; //define constant PI
	const double L = 5; //boundary size 
	double x[N], y[N], theta[N], xvel[N], yvel[N]; //arrays for each x and y coordinates, velocities and direction
	double vel1 = 0.05; //absolute velocity of particle type one (constant)
	double vel2 = 0.10; //absolute velocity of particle type two (constant)
	double r = 1; //interaction radius
	double deltat = 1; //time interval
	double maxt = 1000; //max time period
	double noise = 2; // noise amplitude
	double xvelsum = 0; //sum of velocity x component
	double yvelsum = 0; //sum of velocity y component
	double xvelavg = 0; //average x velocity
	double yvelavg = 0; //average y velocity
	double non = 0; //number of neighbours inside the interaction radius
	double avgtheta = 0; //average theta of neighbourhood
	double difference = 0; //variable for distance between particles


	// random number distribution
	random_device rd;
	mt19937 e2(rd()); //Mersenne Twister 19937 generator
	uniform_real_distribution<> dist(0, 1);


	for (int i = 0; i < N; i++) //intialise each particle
	{
		x[i] = L*dist(e2); //intial x = L * (random variable between 0 and 1)
		y[i] = L*dist(e2); //intial y = L * (random variable between 0 and 1)
		theta[i] = 2 * PI*(dist(e2) - 0.5); //intial theta = 2 * PI * (random variable between 0 and 1)
		xvel[i] = 0; //initialise x velocity
		yvel[i] = 0; //initialise y velocity
		myfile << x[i] << "\t" << y[i] << endl;
	}

	myfile.close();
	plot("set xrange [0:5]"); //set Gnuplot x range
	plot("set yrange [0:5]"); //set Gnuplot y range
	plot("plot 'gnutest.data' using 1:2\n"); //plot data to Gnuplot
	//plot("set term png");
	//plot("set output 'outputstart.png' "); //outputs the starting random distribution
	//plot("replot");
	//plot("set term win"); //sets gnuplot to window format


	for (double t = 0; t < maxt;) //loop for each timestep up to max time
	{
		myfile.open("gnutest.data");

		for (int i = 0; i < N; i++)
		{
			if (i % 2 == 0) //odd and even particles have different velocities
			{
				xvel[i] = vel2*cos(theta[i]); //update velocity x component
				yvel[i] = vel2*sin(theta[i]); //update velocity y component
			}
			else
			{
				xvel[i] = vel1*cos(theta[i]); //update velocity x component
				yvel[i] = vel1*sin(theta[i]); //update velocity y component
			}

			x[i] += xvel[i] * deltat; //update x coordinate
			y[i] += yvel[i] * deltat; //update y coordinate


			//Checks if any particle is outside the boundary
			if (x[i] < 0)
			{
				x[i] = L + x[i];
			}
			if (x[i] > L)
			{
				x[i] = x[i] - L;
			}
			if (y[i] < 0)
			{
				y[i] = L + y[i];
			}
			if (y[i] > L)
			{
				y[i] = y[i] - L;
			}

		}


		//output and plot these positions
		for (int i = 0; i < N; i++)
		{
			myfile << x[i] << "\t" << y[i] << endl;
		}
		myfile.close();
		plot("plot 'gnutest.data' using 1:2\n");


		for (int i = 0; i < N; i++)
		{
			//reset all variables used in loop
			xvelsum = 0;
			yvelsum = 0;
			non = 0;
			xvelavg = 0;
			yvelavg = 0;
			avgtheta = 0;


			for (int j = 0; j < N; j++)
			{

				difference = 0;
				difference = sqrt(pow((x[j] - x[i]), 2.0) + pow((y[j] - y[i]), 2.0)); //distance between particles
				if (i == j) //if they are the same particle add the velocities to the sums
				{
					xvelsum += xvel[i];
					yvelsum += yvel[i];
					non++; //increase size of the neighbourhood
				}
				else if (difference < r) //if the different particles are in the radius add the velocities to the sums
				{
					xvelsum += xvel[j];
					yvelsum += yvel[j];
					non++; //increase size of the neighbourhood
					//cout << "Particles interact" << endl; //test notification if the particles are close enough together
				}
			}
			xvelavg = xvelsum / non; //calculate average x velocity
			yvelavg = yvelsum / non; //calculate average y velocity
			avgtheta = atan2(yvelavg, xvelavg); //calculate average theta
			//cout << "average theta = " << avgtheta << endl; //Used to test the calculation when constructing
			theta[i] = avgtheta + noise*(dist(e2) - 0.5); //update theta

		}

		t += deltat; //increase timestep
		Sleep(12); //add 12 millisecond delay to allow gnuplot to keep up

	}
	plot("set term png");
	plot("set output 'outputend.png' "); //output final result to a PNG file
	plot("replot");


	return 0;
}


