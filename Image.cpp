//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"


// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	unsigned char* Rbuf = new unsigned char[Height * Width];
	unsigned char* Gbuf = new unsigned char[Height * Width];
	unsigned char* Bbuf = new unsigned char[Height * Width];

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
		

	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}


void MyImage::RGBtoHSV(int R, int G, int B, float& H, float& S, float& V) {
	float r = R / 255.0f, g = G / 255.0f, b = B / 255.0f;
	float max_val = fmax(fmax(r, g), b);
	float min_val = fmin(fmin(r, g), b);
	float delta = max_val - min_val;
	V = max_val;
	if (delta <= 0) {
		H = 0;
		S = 0;
	}
	else {
		S = delta / max_val;
		if (r == max_val)
			H = ((g - b) / delta) * 60;
		else if (g == max_val)
			H = (2 + (b - r) / delta) * 60;
		else
			H = (4 + (r - g) / delta) * 60;

		if (H < 0)
			H += 360;
	}
}


unsigned char* MyImage::parseRGB(const char* path) {
	char	ImagePath[_MAX_PATH];
	strcpy(ImagePath, path);
	unsigned char* RGBdata;
	// Create a valid output file pointer
	FILE* IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char* Rbuf = new char[Height * Width];
	char* Gbuf = new char[Height * Width];
	char* Bbuf = new char[Height * Width];

	for (i = 0; i < Width * Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width * Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	RGBdata = new unsigned char[Width * Height * 3];
	for (i = 0; i < Height * Width; i++)
	{
		RGBdata[3 * i] = Bbuf[i];
		RGBdata[3 * i + 1] = Gbuf[i];
		RGBdata[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return RGBdata;
}

/*void MyImage::plotBox(vector<unordered_set<int>> consideredClusters, string args) {
	for (int i = 0; i < consideredClusters.size(); i++) {
		// Convert 1D indices to 2D coordinates and find bounding box corners
		int min_x = Width, min_y = Height, max_x = 0, max_y = 0;
		for (int idx : consideredClusters[i]) {
			int x = idx % Width;
			int y = idx / Width;

			min_x = min(min_x, x);
			min_y = min(min_y, y);
			max_x = max(max_x, x);
			max_y = max(max_y, y);
		}

		int thickness = 3;
		min_x = max(0, min_x - thickness);
		min_y = max(0, min_y - thickness);
		max_x = min(Width - 1, max_x + thickness);
		max_y = min(Height - 1, max_y + thickness);

		// Draw the bounding box (assuming black color for the box)
		for (int y = min_y; y <= max_y; y++) {
			for (int x = min_x; x <= max_x; x++) {
				if (y <= (min_y + thickness - 1) || y >= (max_y - thickness + 1) || x <= (min_x + thickness - 1) || x >= (max_x - thickness + 1)) {
					int idx = (y * Width + x) * 3;
					Data[idx] = 0;     // B value
					Data[idx + 1] = 0;   // G value
					Data[idx + 2] = 255;   // R value
				}
			}
		}

		unordered_map<char, vector<string>> font = {
	{'A', {"111", "101", "111", "101", "101"}},
	{'B', {"110", "101", "110", "101", "110"}},
	{'C', {"111", "100", "100", "100", "111"}},
	{'D', {"110", "101", "101", "101", "110"}},
	{'E', {"111", "100", "110", "100", "111"}},
	{'F', {"111", "100", "111", "100", "100"}},
	{'G', {"111", "100", "101", "101", "111"}},
	{'H', {"101", "101", "111", "101", "101"}},
	{'I', {"111", "010", "010", "010", "111"}},
	{'J', {"111", "010", "010", "010", "100"}},
	{'K', {"101", "101", "110", "101", "101"}},
	{'L', {"100", "100", "100", "100", "111"}},
	{'M', {"101", "111", "111", "101", "101"}},
	{'N', {"101", "111", "111", "111", "101"}},
	{'O', {"111", "101", "101", "101", "111"}},
	{'P', {"111", "101", "111", "100", "100"}},
	{'Q', {"111", "101", "101", "111", "001"}},
	{'R', {"111", "101", "110", "101", "101"}},
	{'S', {"111", "100", "111", "001", "111"}},
	{'T', {"111", "010", "010", "010", "010"}},
	{'U', {"101", "101", "101", "101", "111"}},
	{'V', {"101", "101", "101", "101", "010"}},
	{'W', {"101", "101", "111", "111", "101"}},
	{'X', {"101", "101", "010", "101", "101"}},
	{'Y', {"101", "101", "010", "010", "010"}},
	{'Z', {"111", "001", "010", "100", "111"}},
	{'.', {"000", "000", "000", "000", "010"}},
	{'_', {"000", "000", "000", "000", "010"}}
		};
		size_t pos = args.find_last_of("/\\");
		args = (pos != string::npos) ? args.substr(pos + 1) : args;
		transform(args.begin(), args.end(), args.begin(), ::toupper);
		// Write the string to the image

		// Position to start drawing the string, adjust as needed
		int start_y = max_y - thickness - 10;
		if (start_y < 0) {
			start_y = min_y + thickness;
		}

		int start_x = min_x + thickness;

		// Write the string to the image
		for (char c : args) {
			if (font.find(c) != font.end()) {
				const auto& characterGrid = font[c]; // Fetch the character grid

				for (int y = 0; y < min(10, static_cast<int>(characterGrid.size())); y++) {
					const string& row = characterGrid[y];

					for (int x = 0; x < min(6, static_cast<int>(row.size())); x++) {
						if (row[x] == '1') {
							int cur_x = start_x + x;
							int cur_y = start_y + y;

							if (cur_x >= 0 && cur_x < Width && cur_y >= 0 && cur_y < Height) {
								int idx = (cur_y * Width + cur_x) * 3;
								Data[idx] = 0;     // B value
								Data[idx + 1] = 0;   // G value
								Data[idx + 2] = 255;   // R value
							}
						}
					}
				}
				start_x += 7; // 6 for char width + 1 for spacing
			}
		}

	}
}*/
pair<vector<int>, vector<unordered_set<int>>> MyImage::kMeans(const vector<int>& data, int K) {	
	int dataSize = data.size();
	int maxIterations = 100;
	// Randomly initialize centroids
	vector<int> centroids(K);
	for (int i = 0; i < K; i++) {
		centroids[i] = data[rand() % dataSize];
	}

	vector<int> assignments(dataSize, -1);
	for (int iter = 0; iter < maxIterations; iter++) {
		// Assign each data point to the nearest centroid
		for (int i = 0; i < dataSize; i++) {
			double minDist = (numeric_limits<double>::max)();
			int minIndex = -1;

			for (int j = 0; j < K; j++) {
				int dist = abs(data[i] - centroids[j]);
				if (dist < minDist) {
					minDist = dist;
					minIndex = j;
				}
			}

			assignments[i] = minIndex;
		}

		// Update centroids based on current assignment
		vector<int> newCentroids(K, 0);
		vector<int> counts(K, 0);

		for (int i = 0; i < dataSize; i++) {
			newCentroids[assignments[i]] += data[i];
			counts[assignments[i]]++;
		}

		for (int i = 0; i < K; i++) {
			//newCentroids[i] /= counts[i];
			if (counts[i] != 0) {
				newCentroids[i] /= counts[i];
			}
			else {
				// Handle the case where counts[i] is 0, e.g., by setting the centroid to a default value or re-initializing it
				// For this example, we'll re-initialize it to a random data point:
				newCentroids[i] = data[rand() % dataSize];
			}
		}
		bool converged = true;
		for (int i = 0; i < K; i++) {
			if (abs(newCentroids[i] - centroids[i]) > 0) { // Changed the convergence threshold to 0 for integers
				converged = false;
				break;
			}
		}

		if (converged) {
			break;
		}

		centroids = newCentroids;
	}

	// Build the clusters from assignments
	vector<unordered_set<int>> clusters(K);
	for (int i = 0; i < dataSize; i++) {
		clusters[assignments[i]].insert(data[i]);
	}

	return { centroids, clusters };
}





vector <pair< vector<unordered_set<int>>, string> > MyImage::makeCluster(vector< vector<vector<int>>> &imageHSV, vector< vector<vector<int>>> &objectHSV, unordered_map<int, vector<int>>& pixelToHSV, vector<string> args) {
	vector <pair< vector<unordered_set<int>>, string> > pixelsToDraw;

	for (int objectNo = 0; objectNo < objectHSV.size(); objectNo++) {
		int hueAvg = 0, hueCount = 0, hueMaxPixels = 0;
		cout << endl << "---------- obj "<< objectNo << " --------------------" << endl;
		for (int hue = 0; hue < objectHSV[0][0].size(); hue++) {
			if (objectHSV[objectNo][0][hue] > 0) {
				hueAvg += objectHSV[objectNo][0][hue];
				hueCount++;
				hueMaxPixels = max(hueMaxPixels, objectHSV[objectNo][0][hue]);
			}
			//cout << objectHSV[objectNo][0][hue] << "(" << hue << ")" << " ";
		}

		cout << endl << endl;
		hueAvg /= hueCount;
		cout << "avg: " << hueAvg << endl;
		int hueThreshold = hueAvg * 0.5;
		cout << "hueThreshold: " << hueThreshold << endl;

		cout << endl << "---------- Saturation of obj " << objectNo << " --------------" << endl;

		int satAvg = 0, satCount = 0, satMaxPixels = 0;
		for (int sat = 0; sat < objectHSV[objectNo][1].size(); sat++){
			if (objectHSV[objectNo][1][sat] > 0) {
				satAvg += objectHSV[objectNo][1][sat];
				satCount++;
				satMaxPixels = max(satMaxPixels, objectHSV[objectNo][1][sat]);
			}
			//cout << objectHSV[objectNo][1][sat] << "(" << sat << ")" << " ";
		}
		cout << endl;
		satAvg /= satCount;
		int satThreshold = satAvg * 0.1;

		cout << "Sat threshold: " << satThreshold << endl;

		/*cout << endl << "---------- Hues of obj " << objectNo << " in tthreshold --------------" << endl;

		for (int hue = 0; hue < objectHSV[objectNo][0].size(); hue++)
			if (objectHSV[objectNo][0][hue] >= threshold)
				cout << objectHSV[objectNo][0][hue] << "(" << hue << ")" << " ";*/


		//cout << endl;
		


		cout << endl << "---------- Value of obj " << objectNo << " --------------" << endl;

		int valAvg = 0, valCount = 0, valMaxPixels = 0;
		for (int val = 0; val < objectHSV[objectNo][2].size(); val++) {
			if (objectHSV[objectNo][2][val] > 0) {
				valAvg += objectHSV[objectNo][2][val];
				valCount++;
				valMaxPixels = max(valMaxPixels, objectHSV[objectNo][2][val]);
			}
			//cout << objectHSV[objectNo][2][val] << "(" << val << ")" << " ";
		}
		cout << endl;
		valAvg /= valCount;
		int valThreshold = valAvg * 0.05;	

		cout << "val threshold: " << valThreshold << endl;

		unordered_set<int> hueDegrees, satDegrees, valDegrees, emptySet;

		for (int hue = 0; hue < objectHSV[objectNo][0].size(); hue++) //hue = [0,355]
			if (objectHSV[objectNo][0][hue] >= hueThreshold)
				hueDegrees.insert(hue);

		for (int i = 0; i < objectHSV[objectNo][1].size(); i++) { //sat = [0,100]
			if (objectHSV[objectNo][1][i] >= satThreshold)
				satDegrees.insert(i);
			if (objectHSV[objectNo][2][i] >= valThreshold)
				valDegrees.insert(i);
		}


		vector<int> pixelsToCluster;// , clusters;
		
		for (auto it = pixelToHSV.begin(); it != pixelToHSV.end(); it++) {
			if (hueDegrees.find(it->second[0]) != hueDegrees.end()  && satDegrees.find(it->second[1]) != satDegrees.end() && valDegrees.find(it->second[2]) != valDegrees.end())
				pixelsToCluster.push_back(it->first);
		}


		vector<vector<int>> matrix(Height, vector<int>(Width, 0));
		cout << "pixelsToCluster.size(): " << pixelsToCluster.size() << endl;
		
		if (pixelsToCluster.size() == 0)
			continue;

		/*
		//DB Scan stuff starts here
		int hueClusterThreshod = hueMaxPixels * 0.9;
		vector<int> pixelsToRemove;
		for (int hue = 0; hue < objectHSV[objectNo][0].size(); hue++) //hue = [0,355]
			if (objectHSV[objectNo][0][hue] >= hueClusterThreshod)
				hueDegrees.insert(hue);

		for (int i = 0; i < pixelsToCluster.size(); i++)
			if (hueDegrees.find(pixelToHSV[pixelsToCluster[i]][0]) == hueDegrees.end())
				pixelsToRemove.push_back(pixelsToCluster[i]);

		unordered_set<int> tempSet(pixelsToCluster.begin(), pixelsToCluster.end());

		// Erase elements from set1 that are present in vec2
		for (int val : pixelsToRemove) {
			tempSet.erase(val);
		}

		// Convert the unordered_set back to a vector
		pixelsToCluster.assign(tempSet.begin(), tempSet.end());



		vector<int> clusters = DBSCAN(pixelsToCluster);
		cout << "clusters.size(): " << clusters.size() << endl;
		unordered_map<int, int> clusterToNoOfPixels;
		for (auto val : clusters)
			clusterToNoOfPixels[val]++;

		for (auto it = clusterToNoOfPixels.begin(); it!= clusterToNoOfPixels.end(); it++)
			cout << it->first << " " << it->second << endl;
		
		vector<pair<int, int>> vec(clusterToNoOfPixels.begin(), clusterToNoOfPixels.end());

		sort(vec.begin(), vec.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
			return a.second > b.second;  // '>' for descending order
			});

		for (int i = 0; i < min(1500, clusterToNoOfPixels.size()); i++)
			for (int j = 0; j < clusters.size(); j++) 
				if(clusters[j]== vec[i].first && vec[i].first!=0)
					matrix[pixelsToCluster[j] / Width][pixelsToCluster[j] % Width] = 1;
		*/
			
		
		//Kmeans working code
		int k = 2;
		pair<vector<int>, vector<unordered_set<int>>> result = kMeans(pixelsToCluster, k);
		vector<int> centroids = result.first;
		vector<unordered_set<int>> clusters = result.second;
		int hueClusterThreshod = hueMaxPixels *0.9;
		cout << "hueClusterThreshod: " << hueClusterThreshod << endl;

		hueDegrees = emptySet;
		for (int hue = 0; hue < objectHSV[objectNo][0].size(); hue++) //hue = [0,355]
			if (objectHSV[objectNo][0][hue] >= hueClusterThreshod)
				hueDegrees.insert(hue);
		vector<int> clusterPixelsToRemove;
		for (int i = 0; i < k; i++) {
			int countOfPixelsNotInThreshold = 0;
			for (const int& val : clusters[i]) 
				if (hueDegrees.find(pixelToHSV[val][0]) == hueDegrees.end())
					clusterPixelsToRemove.push_back(val);

			for (int j = 0; j < clusterPixelsToRemove.size(); j++)
				clusters[i].erase(clusterPixelsToRemove[j]);
			cout << "count of pixels in cluster " << i << ": " << clusters[i].size() << endl;
		}
		sort(clusters.begin(), clusters.end(), [](unordered_set<int> a, unordered_set<int> b) {return a.size() > b.size(); });

		vector<unordered_set<int>> clustersToConsider;
		
		//2 clusters for now
		if (clusters[0].size() >= 3 * clusters[1].size())
			clustersToConsider.push_back(clusters[0]);
		else
			clustersToConsider = clusters;

		//for (int i = 0; i < k; i++)
		//	if (i<k-1 && clusters[i].size() > 3 * clusters[i + 1].size() ) //when you create multiple clusters, analyze the condition once
		//		clustersToConsider.push_back(clusters[i]);
		//	else if(clusters[i].size() > 1000)
		
		cout << "clustersToConsider.size(): "<<clustersToConsider.size() << endl;
		pixelsToDraw.push_back({ clustersToConsider, args[objectNo + 1]});
		//plotBox(clustersToConsider, args[objectNo+1]);
		/*for (const int& val : clusters[0])
			matrix[val / Width][val % Width] = 1;
	*/
	
	}
	return pixelsToDraw;
	

}

vector <pair< vector<unordered_set<int>>, string> > MyImage::Modify(vector<string> args)
{

	float H1, S1, V1;

	
	vector< vector<vector<int>>> imageHSV;
	unordered_map<int, vector<int>> pixelToHSV; //= pixel number -> {h, s, v}
	vector<int> H_hist(360, 0);
	vector<int> S_hist(101, 0);  // S and V range is [0, 1] in float, so multiply by 100 to get integer range [0, 100]
	vector<int> V_hist(101, 0);

	//HSV for main image
	for (int i = 0; i < Width * Height; i++) {
		float H, S, V;
		RGBtoHSV(static_cast<int>(Data[3 * i + 2]), static_cast<int>(Data[3 * i + 1]), static_cast<int>(Data[3 * i]), H, S, V);  // Assuming Data is BGR format

		// Clamp H, S, and V to their valid ranges
		H = min(max(H, 0.0f), 359.0f);
		S = min(max(S, 0.0f), 1.0f);
		V = min(max(V, 0.0f), 1.0f);

		H_hist[(int)H]++;
		S_hist[(int)(S * 100)]++;
		V_hist[(int)(V * 100)]++;
		pixelToHSV[i] = { (int)H , (int)(S * 100),(int)(V * 100) };
	}
	imageHSV.push_back({ H_hist ,S_hist, V_hist });

	//HSV for objects
	vector< vector<vector<int>>> objectHSV;
	for (int i = 1; i < args.size(); i++) {
		cout << args[i] << endl;
		vector<int> H_obj(360, 0);
		vector<int> S_obj(101, 0);
		vector<int> V_obj(101, 0);

		//LPSTR imgSrc = const_cast<LPSTR>(args[0].c_str());
		unsigned char* RGBdata = parseRGB(const_cast<LPSTR>(args[i].c_str()));

		for (int j = 0; j < Width * Height; j++) {

			float H, S, V;
			RGBtoHSV(static_cast<int>(RGBdata[3 * j + 2]), static_cast<int>(RGBdata[3 * j + 1]), static_cast<int>(RGBdata[3 * j]), H, S, V);  // Assuming Data is BGR format

			// Clamp H, S, and V to their valid ranges
			H = min(max(H, 0.0f), 359.0f);
			S = min(max(S, 0.0f), 1.0f);
			V = min(max(V, 0.0f), 1.0f);

			H_obj[(int)H]++;
			S_obj[(int)(S * 100)]++;
			V_obj[(int)(V * 100)]++;
		}
		//remove green color from object
		for(int start = 117; start<123; start++)
			H_obj[start] = 0;
		objectHSV.push_back({ H_obj , S_obj, V_obj });

	}
	cout << endl;
	

	return makeCluster(imageHSV, objectHSV, pixelToHSV, args);
}