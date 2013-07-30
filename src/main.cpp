/*
 * main.cpp
 * Copyright (c) 2013, Asil Kaan Bozcuoglu, Institute for Artifical Intelligence, Universitaet Bremen
 * asil@cs.uni-bremen.de
 *
 * All rights reserved.
 *
 * Software License Agreement (BSD License)
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Universitaet Bremen nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>
#include <math.h> 
#include <string>
#include <sstream>     
using namespace std;

int main (int argc, char** argv)
{
	if (argc < 9)
	{
		cout << "Usage: converter [input_folder] [number_of_positive_samples] [number_of_negative_samples] [output_folder] [threshold11] [threshold12] [threshold21] [threshold22]" << endl;
		return -1;
	}

	stringstream ss;
	string argv1(argv[1]);
	string argv4(argv[4]);
	ofstream dataset_writer((char*)(argv4 + "/dataset.csv").c_str());

	//positive samples
	for(int i = 0; i < atof(argv[2]); i++)
	{
		ss << (i + 1);
		string istr = ss.str();
		ss.str("");

		

		ifstream positive_sphere_data_reader((char*)(argv1 + "/success_" + istr + "/sphere_poses.csv").c_str());
		ifstream positive_spatula_data_reader((char*)(argv1 + "/success_" + istr + "/spatula_head_pose.csv").c_str());
		ofstream combined_data_writer((char*)(argv1 + "/success_" + istr + "/combined_data.csv").c_str());
		
		double distance = -1;

		double timestamp = -1;
		bool isFirstThresholdPass = false;
		bool isFirstThresholdHalt = false;
		bool isFirstDataWriting = false;

		double first_timestamp_to_be_recorded = -1;

		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;

		double mean_sphere_vx = 0;
		double mean_sphere_vy = 0;
		double mean_sphere_vz = 0;

		double prev_mean_sphere_vx = -1;
		double prev_mean_sphere_vy = -1;
		double prev_mean_sphere_vz = -1;
		

		double spatula_x = 0;
		double spatula_y = 0;
		double spatula_z = 0;

		double prev_spatula_x = -1;
		double prev_spatula_y = -1;
		double prev_spatula_z = -1;

		double spatula_vx = 0;
		double spatula_vy = 0;
		double spatula_vz = 0;

		double prev_spatula_vx = -1;
		double prev_spatula_vy = -1;
		double prev_spatula_vz = -1;


		double sphere_counter = 0;
		double data_counter = 0;
		double temp = 0;

		double first_timestamp = -1;

		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;
		positive_spatula_data_reader >> temp;	

		while(positive_spatula_data_reader >> timestamp)
		{
			sphere_counter = 0;

			positive_spatula_data_reader >> spatula_x;
			positive_spatula_data_reader >> spatula_y;
			positive_spatula_data_reader >> spatula_z;

			positive_spatula_data_reader >> temp;
			positive_spatula_data_reader >> temp;
			positive_spatula_data_reader >> temp;
			
			bool isAlreadyAllignedTimestamps = false;
			if(data_counter == 0)
			{
				positive_sphere_data_reader >> temp;	
				if(temp == timestamp) 
				{
					isAlreadyAllignedTimestamps = true;
				}
				else
				{
					positive_sphere_data_reader >> temp;
					positive_sphere_data_reader >> temp;
					positive_sphere_data_reader >> temp;
					positive_sphere_data_reader >> temp;
					positive_sphere_data_reader >> temp;
					positive_sphere_data_reader >> temp;
				}
		
			}
			else if(first_timestamp == timestamp)
			{
				positive_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				sphere_counter++;
			}
			else
			{
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
			}
		
			if(isAlreadyAllignedTimestamps)
			{
				positive_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;

				sphere_counter++;
			}

			while(positive_sphere_data_reader >> temp)
			{
				if(temp != timestamp) 
				{
					first_timestamp = temp;
					break;
				}
				positive_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				sphere_counter++;
			}
			
			mean_sphere_x = mean_sphere_x / sphere_counter;
			mean_sphere_y = mean_sphere_y / sphere_counter; 
			mean_sphere_z = mean_sphere_z / sphere_counter;  
				
			distance = pow(pow((mean_sphere_x - spatula_x), 2) + pow((mean_sphere_y - spatula_y), 2) + pow((mean_sphere_z - spatula_z), 2), 1/2);	

			if(prev_spatula_x == -1)
			{
				mean_sphere_vx = 0;
				mean_sphere_vy = 0;
				mean_sphere_vz = 0;

				spatula_vx = 0;
				spatula_vy = 0;
				spatula_vz = 0;
			}
			else
			{
				mean_sphere_vx = mean_sphere_x - prev_mean_sphere_x;
				mean_sphere_vy = mean_sphere_y - prev_mean_sphere_y;
				mean_sphere_vz = mean_sphere_z - prev_mean_sphere_z;

				spatula_vx = spatula_x - prev_spatula_x;
				spatula_vy = spatula_y - prev_spatula_y;
				spatula_vz = spatula_z - prev_spatula_z;
			}

			if(isFirstThresholdPass)
			{
				if(spatula_vz < atof(argv[6]))
				{
					if(isFirstDataWriting)
					{
						first_timestamp_to_be_recorded = timestamp;
						isFirstDataWriting = false;
					}

					combined_data_writer << timestamp - first_timestamp_to_be_recorded 
						<< " " << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
						<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " 1" << endl;

					dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
						<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " ";
				}				
				else break;
		
			}
			else if(fabs(mean_sphere_vx) > atof(argv[5]) || fabs(mean_sphere_vy) > atof(argv[5]) || fabs(mean_sphere_vz) > atof(argv[5]))
			{	
				isFirstThresholdPass = true;			
				isFirstDataWriting = true;
			}

			/*if(isFirstThresholdPass)
			{
				if(isFirstThresholdHalt)
				{
					if(spatula_vz < atof(argv[6]))
					{
						if(isFirstDataWriting)
						{
							first_timestamp_to_be_recorded = timestamp;
							isFirstDataWriting = false;
						}

						combined_data_writer << timestamp - first_timestamp_to_be_recorded 
							<< " " << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
							mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
							spatula_x << " " << spatula_y << " " << spatula_z << " " << 
							spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
							<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " 1" << endl;

						dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
							mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
							spatula_x << " " << spatula_y << " " << spatula_z << " " << 
							spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
							<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " ";
					}				
					else break;
				}
				else if(spatula_vz < atof(argv[5]))
				{		
					isFirstThresholdHalt = true;
					isFirstDataWriting = true;
				}
		
			}
			else if(spatula_vz > atof(argv[5]))		
				isFirstThresholdPass = true;*/

			prev_spatula_x = spatula_x;
			prev_spatula_y = spatula_y;
			prev_spatula_z = spatula_z;

			prev_mean_sphere_x = mean_sphere_x;
			prev_mean_sphere_y = mean_sphere_y;
			prev_mean_sphere_z = mean_sphere_z;

			prev_spatula_vx = spatula_vx;
			prev_spatula_vy = spatula_vy;
			prev_spatula_vz = spatula_vz;

			prev_mean_sphere_vx = mean_sphere_vx;
			prev_mean_sphere_vy = mean_sphere_vy;
			prev_mean_sphere_vz = mean_sphere_vz;

			data_counter++;	
		}

		dataset_writer << "1" << endl;
	}

	//negative samples
	for(int i = 0; i < atof(argv[3]); i++)
	{
		ss << (i + 1);
		string istr = ss.str();
		ss.str("");


		ifstream negative_sphere_data_reader((char*)(argv1 + "/fail_push_off_" + istr + "/sphere_poses.csv").c_str());
		ifstream negative_spatula_data_reader((char*)(argv1 + "/fail_push_off_" + istr + "/spatula_head_pose.csv").c_str());
		ofstream combined_data_writer((char*)(argv1 + "/fail_push_off_" + istr + "/combined_data.csv").c_str());

		double distance = -1;

		double timestamp = -1;
		bool isFirstThresholdPass = false;
		bool isFirstThresholdHalt = false;
		bool isFirstDataWriting = false;

		double first_timestamp_to_be_recorded = -1;
		
		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;

		double mean_sphere_vx = 0;
		double mean_sphere_vy = 0;
		double mean_sphere_vz = 0;

		double prev_mean_sphere_vx = -1;
		double prev_mean_sphere_vy = -1;
		double prev_mean_sphere_vz = -1;
		

		double spatula_x = 0;
		double spatula_y = 0;
		double spatula_z = 0;

		double prev_spatula_x = -1;
		double prev_spatula_y = -1;
		double prev_spatula_z = -1;

		double spatula_vx = 0;
		double spatula_vy = 0;
		double spatula_vz = 0;

		double prev_spatula_vx = -1;
		double prev_spatula_vy = -1;
		double prev_spatula_vz = -1;


		double sphere_counter = 0;
		double data_counter = 0;
		double temp = 0;

		double first_timestamp = -1;

		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
		negative_spatula_data_reader >> temp;
			

		while(negative_spatula_data_reader >> timestamp)
		{
			sphere_counter = 0;

			negative_spatula_data_reader >> spatula_x;
			negative_spatula_data_reader >> spatula_y;
			negative_spatula_data_reader >> spatula_z;

			negative_spatula_data_reader >> temp;
			negative_spatula_data_reader >> temp;
			negative_spatula_data_reader >> temp;
			
			bool isAlreadyAllignedTimestamps = false;
			if(data_counter == 0)
			{
				negative_sphere_data_reader >> temp;
				if(temp == timestamp) 
				{
					isAlreadyAllignedTimestamps = true;
				}
				else
				{
					negative_sphere_data_reader >> temp;
					negative_sphere_data_reader >> temp;
					negative_sphere_data_reader >> temp;
					negative_sphere_data_reader >> temp;
					negative_sphere_data_reader >> temp;
					negative_sphere_data_reader >> temp;
				}
		
			}
			else if(first_timestamp == timestamp)
			{
				negative_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				sphere_counter++;
			}
			else
			{
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
			}
		
		
			if(isAlreadyAllignedTimestamps)
			{
				negative_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				sphere_counter++;
			}

			while(negative_sphere_data_reader >> temp)
			{
				if(temp != timestamp) 
				{
					first_timestamp = temp;
					break;
				}
				negative_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_z += temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				sphere_counter++;
			}

			mean_sphere_x /= sphere_counter;
			mean_sphere_y /= sphere_counter; 
			mean_sphere_z /= sphere_counter; 

			distance = pow(pow((mean_sphere_x - spatula_x), 2) + pow((mean_sphere_y - spatula_y), 2) + pow((mean_sphere_z - spatula_z), 2), 1/2); 

			if(prev_spatula_x == -1)
			{
				mean_sphere_vx = 0;
				mean_sphere_vy = 0;
				mean_sphere_vz = 0;

				spatula_vx = 0;
				spatula_vy = 0;
				spatula_vz = 0;
			}
			else
			{
				mean_sphere_vx = mean_sphere_x - prev_mean_sphere_x;
				mean_sphere_vy = mean_sphere_y - prev_mean_sphere_y;
				mean_sphere_vz = mean_sphere_z - prev_mean_sphere_z;

				spatula_vx = spatula_x - prev_spatula_x;
				spatula_vy = spatula_y - prev_spatula_y;
				spatula_vz = spatula_z - prev_spatula_z;
			}

			if(isFirstThresholdPass)
			{
				if(mean_sphere_vz > atof(argv[8]))
				{
					if(isFirstDataWriting)
					{
						first_timestamp_to_be_recorded = timestamp;
						isFirstDataWriting = false;
					}

					combined_data_writer << timestamp - first_timestamp_to_be_recorded 
						<< " " << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
						<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " 0" << endl;

					dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
						<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " ";
				}				
				else break;
		
			}
			else if(fabs(mean_sphere_vx) > atof(argv[5]) || fabs(mean_sphere_vy) > atof(argv[5]) || fabs(mean_sphere_vz) > atof(argv[5]))
			{		
				isFirstThresholdPass = true;			
				isFirstDataWriting = true;
			}

			/*if(isFirstThresholdPass)
			{
				if(isFirstThresholdHalt)
				{
					
					if(spatula_vz > atof(argv[8]))
					{
						if(isFirstDataWriting)
						{
							first_timestamp_to_be_recorded = timestamp;
							isFirstDataWriting = false;
						}

						combined_data_writer << timestamp - first_timestamp_to_be_recorded 
							<< " " << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
							mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
							spatula_x << " " << spatula_y << " " << spatula_z << " " << 
							spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
							<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " 0" << endl;

						dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
							mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
							spatula_x << " " << spatula_y << " " << spatula_z << " " << 
							spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << spatula_vx - mean_sphere_vx
							<< " " << spatula_vy - mean_sphere_vy << " " << spatula_vz - mean_sphere_vz << " ";
					}				
					else break;
				}
				else if(spatula_vz < atof(argv[7]))
				{		
					isFirstThresholdHalt = true;
					isFirstDataWriting = true;
				}
		
			}
			else if(spatula_vz > atof(argv[7]))		
				isFirstThresholdPass = true;*/

			prev_spatula_x = spatula_x;
			prev_spatula_y = spatula_y;
			prev_spatula_z = spatula_z;

			prev_mean_sphere_x = mean_sphere_x;
			prev_mean_sphere_y = mean_sphere_y;
			prev_mean_sphere_z = mean_sphere_z;

			prev_spatula_vx = spatula_vx;
			prev_spatula_vy = spatula_vy;
			prev_spatula_vz = spatula_vz;

			prev_mean_sphere_vx = mean_sphere_vx;
			prev_mean_sphere_vy = mean_sphere_vy;
			prev_mean_sphere_vz = mean_sphere_vz;

			data_counter++;	
		}

		dataset_writer << "0" << endl;
	}

	return 0;
}
