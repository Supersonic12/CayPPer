#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

using namespace std;

std::vector<filesystem::path> listDirectory(filesystem::path dirPath)
{

	// Checking existence of directory
	if (!filesystem::exists(dirPath))
	{
		cerr << "NonExistent folder";
		return {};
	}

	// Array of files
	std::vector<filesystem::path> files;

	// add files to array
	for (const auto &entry : filesystem::directory_iterator(dirPath))
	{
		if (!filesystem::is_directory(entry.path()))
		{
			files.push_back(entry.path());
		}
	}
	// Check extensions
	std::vector<filesystem::path> files_to_remove;
	for (const auto &entry : files)
	{
		if (entry.extension() != ".png" && entry.extension() != ".jpg" && entry.extension() != ".jpeg")
		{
			files_to_remove.push_back(entry);
		}
	}
    //erase unwanted ones
	files.erase(
		std::remove_if(files.begin(),
					   files.end(),
					   [&files_to_remove](const filesystem::path &p)
					   {
						   return std::find(files_to_remove.begin(), files_to_remove.end(), p) != files_to_remove.end();
					   }),
		files.end());

	return files;
}
