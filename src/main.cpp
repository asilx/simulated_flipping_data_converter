#include <iostream>
#include <fstream>
using namespace std;

int main (int argc, char** argv)
{
	if (argc < 9)
	{
		cout << "Usage: converter [input_folder] [number_of_positive_samples] [number_of_negative_samples] [output_file] [threshold11] [threshold12] [threshold21] [threshold22]" << endl;
		return -1;
	}

	ofstream dataset_writer(argv[1] + "/dataset.csv");

	//positive samples
	for(int i = 0; i < atoi(argv[2]); i++)
	{
		ifstream positive_sphere_data_reader(argv[1] + "/success_" + itoa(i) + "/sphere_poses.csv");
		ifstream positive_spatula_data_reader(argv[1] + "/success_" + itoa(i) + "/spatula_head_poses.csv");
		ofstream combined_data_writer(argv[1] + "/success_" + itoa(i) + "/combined_data.csv");

		double timestamp = -1;
		bool isFirstThresholdPass = false;
		
		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;

		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;
		

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
					break;
				}
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
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				sphere_counter++;
			}

			while(positive_sphere_data_reader >> temp)
			{
				if(temp != timestamp) break;
				positive_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				positive_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				positive_sphere_data_reader >> temp;
				sphere_counter++;
			}
			
			mean_sphere_x /= sphere_counter;
			mean_sphere_y /= sphere_counter; 
			mean_sphere_z /= sphere_counter;  

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
				mean_sphere_vx = mean_sphere_x - prev_sphere_x;
				mean_sphere_vy = mean_sphere_y - prev_sphere_y;
				mean_sphere_vz = mean_sphere_z - prev_sphere_z;

				spatula_vx = spatula_x - prev_spatula_X;
				spatula_vy = spatula_y - prev_spatula_y;
				spatula_vz = spatula_z - prev_spatula_z;
			}

			if(isFirstThresholdPass)
			{
				if(spatula_vz < atoi(argv[6])
				{
					combined_data_writer << timestamp << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << endl;

					dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " ";
				}				
				else break;		
			}
			else if(spatula_vz > atoi(argv[5]))		
				isFirstThresholdPass = true;

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

		dataset_writer << endl;
	}

	//negative samples
	for(int i = 0; i < atoi(argv[3]); i++)
	{
		ifstream negative_sphere_data_reader(argv[1] + "/fail_" + itoa(i) + "/sphere_poses.csv");
		ifstream negative_spatula_data_reader(argv[1] + "/fail_" + itoa(i) + "/spatula_head_poses.csv");
		ofstream combined_data_writer(argv[1] + "/fail_" + itoa(i) + "/combined_data.csv");

		double timestamp = -1;
		bool isFirstThresholdPass = false;
		
		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;

		double mean_sphere_x = 0;
		double mean_sphere_y = 0;
		double mean_sphere_z = 0;

		double prev_mean_sphere_x = -1;
		double prev_mean_sphere_y = -1;
		double prev_mean_sphere_z = -1;
		

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
					break;
				}
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
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				sphere_counter++;
			}

			while(negative_sphere_data_reader >> temp)
			{
				if(temp != timestamp) break;
				negative_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_y += temp;
				negative_sphere_data_reader >> temp;
				mean_sphere_x += temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				negative_sphere_data_reader >> temp;
				sphere_counter++;
			}
			
			mean_sphere_x /= sphere_counter;
			mean_sphere_y /= sphere_counter; 
			mean_sphere_z /= sphere_counter;  

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
				mean_sphere_vx = mean_sphere_x - prev_sphere_x;
				mean_sphere_vy = mean_sphere_y - prev_sphere_y;
				mean_sphere_vz = mean_sphere_z - prev_sphere_z;

				spatula_vx = spatula_x - prev_spatula_X;
				spatula_vy = spatula_y - prev_spatula_y;
				spatula_vz = spatula_z - prev_spatula_z;
			}

			if(isFirstThresholdPass)
			{
				if(spatula_vz < atoi(argv[6])
				{
					combined_data_writer << timestamp << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " " << endl;

					dataset_writer << mean_sphere_x << " " << mean_sphere_y << " " << mean_sphere_z << " " <<
						mean_sphere_vx << " " << mean_sphere_vy << " " << mean_sphere_vz << " " <<
						spatula_x << " " << spatula_y << " " << spatula_z << " " << 
						spatula_vx << " " << spatula_vy << " " << spatula_vz << " ";
				}				
				else break;		
			}
			else if(spatula_vz > atoi(argv[5]))		
				isFirstThresholdPass = true;

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

		dataset_writer << endl;
	}

	return 0;
}
