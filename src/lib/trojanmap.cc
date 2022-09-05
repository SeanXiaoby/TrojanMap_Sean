#include "trojanmap.h"

//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return
 * -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string &id)
{
  double res = data[id].lat;
  return res;
}

/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist,
 * return -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string &id)
{
  double res = data[id].lon;
  return res;
}

/**
 * GetName: Get the name of a Node given its id. If id does not exist, return
 * "NULL".
 *
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string &id) { 
  if(id == "")
    return "";
  else {
      std::string name = data[id].name;
      return name;
  }
}

/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return
 * an empty vector.
 *
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string &id)
{
  std::vector<std::string> res;
  if (data.count(id) == 0)
  {
    return res;
  }
  else
  {
    res = data[id].neighbors;
  }
  return res;
}

/**
 * GetID: Given a location name, return the id.
 * If the node does not exist, return an empty string.
 *
 * @param  {std::string} name          : location name
 * @return {int}  : id
 */
std::string TrojanMap::GetID(const std::string &name)
{
  std::string res = "";

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.name == name)
    {
      res = iter->second.id;
      break;
    }
  }
  return res;
}

/**
 * GetPosition: Given a location name, return the position. If id does not
 * exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name)
{
  std::pair<double, double> results(-1, -1);

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (name == iter->second.name)
    {
      results.first = iter->second.lat;
      results.second = iter->second.lon;
      break;
    }
  }

  return results;
}

/**
 * CalculateEditDistance: Calculate edit distance between two location names
 *
 */
int TrojanMap::CalculateEditDistance(std::string a, std::string b)
{
  int length1 = a.length();
  int length2 = b.length();
  std::vector<std::vector<int>> dp(length2 + 1, std::vector<int>(length1 + 1, 0)); //(row, vector<int>(col, 1))
  for (auto i = 0; i < length1 + 1; i++)
    dp[0][i] = i;
  for (auto j = 0; j < length2 + 1; j++)
    dp[j][0] = j;

  for (auto i = 1; i < length1 + 1; i++)
  {
    for (auto j = 1; j < length2 + 1; j++)
    {
      if (a[i - 1] == b[j - 1])
        dp[j][i] = dp[j - 1][i - 1];
      else
        dp[j][i] = std::min(dp[j - 1][i - 1], std::min(dp[j - 1][i], dp[j][i - 1])) + 1;
    }
  }
  return dp[length2][length1];
}

/**
 * FindClosestName: Given a location name, return the name with smallest edit
 * distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : similar name
 */
std::string TrojanMap::FindClosestName(std::string name)
{
  std::string tmp = "";
  std::string final = "";
  auto comparator = name.size();
  for (auto i = data.begin(); i != data.end(); i++)
  {
    tmp = i->second.name;
    auto distance = CalculateEditDistance(name, tmp);
    if (distance < comparator)
    {
      comparator = distance;
      final = tmp;
    }
  }
  return final;
}

/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name)
{
  std::vector<std::string> results;

  std::string lower_name = StringToLower(name);

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    std::string lower_target = StringToLower(iter->second.name);
    if (lower_target.size() < lower_name.size())
    {
      continue;
    }
    else
    {
      if (lower_target.substr(0, lower_name.size()) == lower_name)
      {
        results.push_back(iter->second.name);
      }
    }
  }

  return results;
}

/**
 * GetAllCategories: Return all the possible unique location categories, i.e.
 * there should be no duplicates in the output.
 *
 * @return {std::vector<std::string>}  : all unique location categories
 */
std::vector<std::string> TrojanMap::GetAllCategories()
{
  std::unordered_set<std::string> setRes;

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.attributes.size() != 0)
    {

      std::unordered_set<std::string> tempSet = iter->second.attributes;

      for (auto iter2 = tempSet.begin(); iter2 != tempSet.end(); iter2++)
      {
        setRes.insert(*iter2);
      }
    }
  }

  std::vector<std::string> vecRes;
  for (auto iter = setRes.begin(); iter != setRes.end(); iter++)
  {
    vecRes.push_back(*iter);
  }
  return vecRes;
}

/**
 * GetAllLocationsFromCategory: Return all the locations of the input category (i.e.
 * 'attributes' in data.csv). If there is no location of that category, return
 * (-1, -1). The function should be case-insensitive.
 *
 * @param  {std::string} category          : category name (attribute)
 * @return {std::pair<double, double>}     : (lat, lon)
 */
std::vector<std::pair<double, double>> TrojanMap::GetAllLocationsFromCategory(
    std::string category)
{
  std::vector<std::pair<double, double>> vecRes;
  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.attributes.count(category) != 0)
    {
      std::pair<double, double> temp;
      temp.first = iter->second.lat;
      temp.second = iter->second.lon;
      vecRes.push_back(temp);
    }
  }

  return vecRes;
}

/**
 * GetLocationRegex: Given the regular expression of a location's name, your
 * program should first check whether the regular expression is valid, and if so
 * it returns all locations that match that regular expression.
 *
 * @param  {std::regex} location name      : the regular expression of location
 * names
 * @return {std::pair<double, double>}     : (lat, lon)
 */
std::vector<std::pair<double, double>> TrojanMap::GetLocationRegex(std::regex location)
{
  std::vector<std::pair<double, double>> res;

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (std::regex_search(iter->second.name, location))
    {
      res.push_back({iter->second.lat, iter->second.lon});
    }
  }

  return res;
}

/**
 * GetLocationRegex: Given the regular expression of a location's name, your
 * program should first check whether the regular expression is valid, and if so
 * it returns all locations that match that regular expression.
 *
 * @param  {std::string} location name      : the regular expression of location(string)
 * names
 * @return {std::pair<double, double>}     : (lat, lon)
 */
std::vector<std::pair<double, double>> TrojanMap::GetLocationRegex(std::string location)
{
  try
  {
    // If s is invalid, the execution will jump into the catch block.
    auto r = std::regex(location);
    return GetLocationRegex(r);
  }
  catch (const std::exception &e)
  {
    // If we are here, it means the regex was invalid and an exception happened.
    // you can return an empty vector or anything else that indicates an error..
    std::vector<std::pair<double, double>> r;
    r.push_back({-1, -1});
    return r;
  }
}

/**
 * CalculateDistance: Get the distance between 2 nodes.
 *
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id,
                                    const std::string &b_id)
{
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2), 2.0) + cos(a.lat * M_PI / 180.0) *
                                           cos(b.lat * M_PI / 180.0) *
                                           pow(sin(dlon / 2), 2.0);
  double c = 2 * asin(std::min(1.0, sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations
 * inside the vector.
 *
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path)
{
  // Do not change this function
  double sum = 0;
  for (int i = 0; i < int(path.size()) - 1; i++)
  {
    sum += CalculateDistance(path[i], path[i + 1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path
 * which is a list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name)
{

  std::vector<std::string> path;

  if (!(FindLocationName(location1_name) && FindLocationName(location2_name)))
  {
    return path;
  }

  std::unordered_map<std::string, double> mapDist;        // Store the min distance from root
  std::unordered_map<std::string, std::string> mapParent; // Store the Parent of each node
  std::unordered_map<std::string, bool> mapVisited;       // Store the visted marks for each node
  int visited_size = 0;
  std::string root_id = GetID(location1_name), target_id = GetID(location2_name);

  // Initialize the maps
  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    mapDist[iter->first] = __INT_MAX__;
    mapParent[iter->first] = "-1";
    mapVisited[iter->first] = false;
  }

  mapDist[root_id] = 0;
  mapParent[root_id] = "-1";

  while (visited_size < data.size())
  {
    std::pair<std::string, double> minVertex = {"-1", __INT_MAX__};

    if (visited_size == 0)
    {
      minVertex.first = root_id;
      minVertex.second = 0;
    }
    else
    {
      for (auto iter = mapVisited.begin(); iter != mapVisited.end(); iter++)
      {
        if (iter->second == true)
        {
          continue;
        }
        else
        {
          if (mapDist[iter->first] < minVertex.second)
          {
            minVertex.first = iter->first;
            minVertex.second = mapDist[iter->first];
          }
        }
      }
    }

    mapVisited[minVertex.first] = true;
    visited_size++;

    if (minVertex.first == target_id)
    {
      break;
    }

    std::vector<std::string> vecNeighbors = GetNeighborIDs(minVertex.first);

    for (auto iter = vecNeighbors.begin(); iter != vecNeighbors.end(); iter++)
    {
      if (mapDist[*iter] != __INT_MAX__ && mapVisited[*iter] == false)
      {
        if (mapDist[minVertex.first] + CalculateDistance(minVertex.first, *iter) <= mapDist[*iter])
        {
          mapDist[*iter] = mapDist[minVertex.first] + CalculateDistance(minVertex.first, *iter);
          mapParent[*iter] = minVertex.first;
        }
      }
      else if (mapDist[*iter] == __INT_MAX__ && mapVisited[*iter] == false)
      {
        mapDist[*iter] = mapDist[minVertex.first] + CalculateDistance(minVertex.first, *iter);
        mapParent[*iter] = minVertex.first;
      }
    }
  }

  std::string tempId = target_id;
  while(tempId != "-1"){
    path.push_back(tempId);
    tempId = mapParent[tempId];
  }

  std::reverse(path.begin(), path.end());

  return path;
}

/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest
 * path which is a list of id. Hint: Do the early termination when there is no
 * change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name)
{
  std::vector<std::string> path;
  std::unordered_map<std::string, double> mapDist;
  std::unordered_map<std::string, std::string> mapParent;

  std::string root_id = GetID(location1_name), target_id = GetID(location2_name);

  if(!(FindLocationName(location1_name) && FindLocationName(location2_name))){
    return path;
  }

  for(auto iter = data.begin(); iter!=data.end(); iter++){
    mapDist[iter->first] = __INT_MAX__;
    mapParent[iter->first] = "0";
  }

  mapDist[root_id] = 0;
  mapParent[root_id] = "-1";

  for(int i = 0; i<(data.size()-1); ++i){
    bool bIsChanged = false;

    for(auto iter = data.begin(); iter!=data.end(); iter++){
      std::vector<std::string> vecNeighbors = GetNeighborIDs(iter->first);
      for(auto child = vecNeighbors.begin(); child!=vecNeighbors.end(); child ++){
        if(mapDist[iter->first] == __INT_MAX__)  continue;
        double edge = CalculateDistance(iter->first, *child);
        if(mapDist[iter->first] + edge < mapDist[*child]){
          mapDist[*child] = mapDist[iter->first] + edge;
          mapParent[*child] = iter->first;
          bIsChanged = true;
          //std::cout<<"Modified!"<<std::endl;
        }
      }
    }

    if(!bIsChanged) break;
  }

  std::string tempId = target_id;
  while(tempId != "-1" && path.size() < data.size()){
    path.push_back(tempId);
    tempId = mapParent[tempId];
  }
  std::reverse(path.begin(), path.end());

  return path;
}

/**
 * Traveling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of
 * total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_Brute_force(std::vector<std::string> location_ids)
{
  std::pair<double, std::vector<std::vector<std::string>>> records;
  records.first = __INT_MAX__;

  if(location_ids.size() == 0){
    return records;
  }

  std::pair< double, std::vector<std::string> > curResult;
  curResult.first = 0;

  BruteForce_helper(location_ids, records, curResult);

  //std::cout<<"Brute force ended!"<<std::endl;
  for(int i = 0; i<records.second.size(); ++i){
    auto &tempPath = records.second[i];
    if(tempPath.size() > 1){
      records.first += CalculateDistance(tempPath[0], tempPath[tempPath.size()-1]);
      tempPath.push_back(tempPath[0]);
    }
  }
  records.first = CalculatePathLength(records.second[records.second.size()-1]);

  return records;
}

// Brute_force permutation helper for Item 9
void TrojanMap::BruteForce_helper(std::vector<std::string> &location_ids, 
                                    std::pair<double, std::vector<std::vector<std::string> > > &results, 
                                    std::pair<double, std::vector<std::string> > curPath){

  // If we hit the leaf, return the results
  if(curPath.second.size() == location_ids.size()){
    if(curPath.first < results.first){
      results.second.push_back(curPath.second);
      results.first = curPath.first;
    }
    return;
  }

  for(auto iter_id = location_ids.begin(); iter_id!= location_ids.end(); iter_id++){
    if(std::find(curPath.second.begin(), curPath.second.end(), *iter_id) != curPath.second.end()){
      continue;
    }

    double temp = curPath.first;
    if(curPath.second.size() == 0){
      curPath.first = 0;
    }else{
      curPath.first += CalculateDistance(curPath.second[curPath.second.size()-1], *iter_id);
    }
    curPath.second.push_back(*iter_id);

    BruteForce_helper(location_ids, results, curPath);

    curPath.first = temp;
    curPath.second.pop_back();
  }

}


std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_Backtracking(std::vector<std::string> location_ids)
{
  std::pair<double, std::vector<std::vector<std::string>>> records;
  records.first = __INT_MAX__;

  if(location_ids.size() == 0){
    return records;
  }

  std::pair< double, std::vector<std::string> > curResult;
  curResult.first = 0;

  Backtracking_helper(location_ids, records, curResult);

  // std::vector<std::string> &tempPath = records.second[records.second.size()-1];
  for(int i = 0; i<records.second.size(); ++i){
    auto &tempPath = records.second[i];
    if(tempPath.size() > 1){
      records.first += CalculateDistance(tempPath[0], tempPath[tempPath.size()-1]);
      tempPath.push_back(tempPath[0]);
    }
  }
  records.first = CalculatePathLength(records.second[records.second.size()-1]);

  return records;
}

// Brute_force w/ backtracking permutation helper for Item 9
void TrojanMap::Backtracking_helper(std::vector<std::string> &location_ids, 
                                    std::pair<double, std::vector<std::vector<std::string> > > &results, 
                                    std::pair<double, std::vector<std::string> > curPath){

  // If we hit the leaf, return the results
  if(curPath.second.size() == location_ids.size()){
    if(curPath.first < results.first){
      // if(curPath.second.size()>1){
      //   curPath.second.push_back(curPath.second[0]);
      //   curPath.first += CalculateDistance(curPath.second[0], curPath.second[curPath.second.size()-1]);
      // }
      results.second.push_back(curPath.second);
      results.first = curPath.first;
      //std::cout<<"Current distance is: "<<curPath.first<<std::endl;
    }
    return;
  }

  // Early backtracking part
  if(curPath.first >= results.first){
    return;
  }

  for(auto iter_id = location_ids.begin(); iter_id!= location_ids.end(); iter_id++){
    if(std::find(curPath.second.begin(), curPath.second.end(), *iter_id) != curPath.second.end()){
      continue;
    }

    double temp = curPath.first;
    if(curPath.second.size() == 0){
      curPath.first = 0;
    }else{
      curPath.first += CalculateDistance(curPath.second[curPath.second.size()-1], *iter_id);
    }
    curPath.second.push_back(*iter_id);

    Backtracking_helper(location_ids, results, curPath);

    curPath.first = temp;
    curPath.second.pop_back();
  }

}


/**
 * Traveling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 * 2-opt Algprithm solution
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of
 * total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>>
TrojanMap::TravelingTrojan_2opt(std::vector<std::string> location_ids)
{
  std::pair<double, std::vector<std::vector<std::string>>> records;
  records.first = 0;

  if(location_ids.size() == 0){
    return records;
  }else if(location_ids.size() == 1){
    auto temp = location_ids;
    records.second.push_back(temp);
    return records;
  }

  auto curPath = location_ids;
  curPath.push_back(curPath[0]);
  records.first = CalculatePathLength(curPath);
  records.second.push_back(curPath);
  curPath.pop_back();
  int nCount = 0, nMaxSwaps = 10000, nMaxPath = MyFac(location_ids.size()), nNoOpt = 0;

  while(nNoOpt < nMaxPath){
    nCount++;

    // Randomly swap 2 ids
    int ptr1 = MyRand(0, curPath.size()-1), ptr2 = MyRand(0, curPath.size()-1);
    if(ptr1 == ptr2)  continue;

    auto tempID = curPath[ptr1];
    curPath[ptr1] = curPath[ptr2];
    curPath[ptr2] = tempID;

    // Compare path length
    double curLength = CalculatePathLength(curPath);
    if(curLength<records.first){
      records.first = curLength;
      // records.second.push_back(curPath);
      auto tempPath = curPath;
      tempPath.push_back(tempPath[0]);
      records.second.push_back(tempPath);
      nNoOpt = 0;
    }
    nNoOpt ++;
  }
  records.first = CalculatePathLength(records.second[records.second.size()-1]);

  records.second[records.second.size()-1] = ReorganizePath(location_ids[0], records.second[records.second.size()-1]);

  return records;
}

// Generate random numbers between [a, b]
int TrojanMap::MyRand(int a, int b){
  return rand() % (b - a +1) + a; 
}

// Calculate Factorial
int TrojanMap::MyFac(int i){
  if(i <= 1){
    return 1;
  }else{
    return i * MyFac(i-1);
  }
}

// Reorganize Output path orders
std::vector<std::string> TrojanMap::ReorganizePath(std::string first_item, std::vector<std::string> input_path){
  std::vector<std::string> res;
  if(input_path.size() <= 1){
    return input_path;
  }

  input_path.pop_back();
  auto pos = std::find(input_path.begin(), input_path.end(), first_item);

  while(res.size() < input_path.size()){
    res.push_back(*pos);
    if(pos == input_path.end()-1){
      pos = input_path.begin();
    }else{
      pos++;
    }
  }

  res.push_back(res[0]);

  return res;
}


/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 *
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(
    std::string locations_filename)
{
  std::vector<std::string> location_names_from_csv;
  std::fstream fin;
  fin.open(locations_filename, std::ios::in);

  std::string line;
  getline(fin, line);
  while (getline(fin, line))
  {

    // std::cout <<"locations!"<< word;
    // line.erase(std::remove(line.begin(), line.end(), ','), line.end());
    location_names_from_csv.push_back(line);
  }
  fin.close();
  return location_names_from_csv;
}

/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 *
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(
    std::string dependencies_filename)
{
  std::vector<std::vector<std::string>> dependencies_from_csv;
  std::fstream fin;
  fin.open(dependencies_filename, std::ios::in);

  std::string line;
  std::string word;
  getline(fin, line);
  while (getline(fin, line))
  {
    std::stringstream m(line);
    dependencies_from_csv.push_back(std::vector<std::string>());
    while (getline(m, word, ','))
    {
      // std::cout <<"dependencies!"<< word;
      dependencies_from_csv.back().push_back(word);
    }
  }
  fin.close();
  return dependencies_from_csv;
}

/**
 * DeliveringTrojan: Given a vector of location names, it should return a
 * sorting of nodes that satisfies the given dependencies. If there is no way to
 * do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     :
 * prerequisites
 *
 *
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(
    std::vector<std::string> &locations,
    std::vector<std::vector<std::string>> &dependencies)
{
  // std::vector<std::string> result;
  std::unordered_map<std::string, std::vector<std::string>> hash_map;
  for (auto &iter : locations){
    hash_map[iter] = std::vector<std::string>{};
    // std::cout<<"!!!iter!!!" << iter<<std::endl;
  }
  for (auto &dependency : dependencies){
    hash_map[dependency[0]].push_back(dependency[1]);
    // std::cout<<"!!!iter!!!" << dependency[1]<<std::endl;
  }
  std::map<std::string, int> marks;
  std::vector<std::string> topo_list;
  // std::cout << "hash_map size  " << hash_map.size() <<std::endl;
  for (int i = 0; i < locations.size(); i++){
    if (marks[locations[i]] != 1)
      DFS_helper(locations[i], hash_map, marks, topo_list);
  }
  std::reverse(topo_list.begin(), topo_list.end());

  return topo_list;
}

void TrojanMap::DFS_helper(std::string &root, std::unordered_map<std::string, std::vector<std::string>> &hash_map, std::map<std::string, int> &marks, std::vector<std::string> &topo_list)
{
  marks[root] = 1;
  std::cout << "!!!root!!!  " << root << std::endl;
  for (auto &child : hash_map[root])
  {
    // std::cout<<"!!!root!!!  " << root <<std::endl;
    // std::cout<<"!!!child!!!  " << child <<std::endl;
    // std::cout<<"!!!marks[child]!!!  " << marks[child] <<std::endl;
    if (marks[child] != 1)
      DFS_helper(child, hash_map, marks, topo_list);
  }
  topo_list.push_back(root);
  // std::cout<<"!!!iter!!!" << root <<std::endl;
}

/**
 * inSquare: Give a id retunr whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square)
{

  if (square.size() != 4)
  {
    return false;
  }

  if (square[0] > square[1] || square[2] < square[3])
  {
    return false;
  }

  if (data[id].lat >= square[3] && data[id].lat <= square[2] && data[id].lon >= square[0] && data[id].lon <= square[1])
  {
    return true;
  }

  return false;
}

/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location
 * ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square
 * area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the
 * square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square)
{
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;

  if (square.size() != 4)
  {
    return subgraph;
  }

  if (square[0] > square[1] || square[2] < square[3])
  {
    return subgraph;
  }

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.lat >= square[3] && iter->second.lat <= square[2] && iter->second.lon >= square[0] && iter->second.lon <= square[1])
    {
      subgraph.push_back(iter->first);
    }
  }

  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true
 * if there is a cycle path inside the square, false otherwise.
 *
 * @param {std::vector<std::string>} subgraph: list of location ids in the
 * square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */
bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph,
                               std::vector<double> &square)
{
  if (subgraph.empty())
  {
    return false;
  }

  bool bFlag = false;
  std::stack<std::string> tempStack;
  std::map<std::string, bool> mapVisited;
  int nVisisted = 0;

  for (auto iter = subgraph.begin(); iter != subgraph.end(); iter++)
  {
    mapVisited[*iter] = false;
  }

  std::string root_id = subgraph[0];
  tempStack.push(root_id);
  mapVisited[root_id] = true;
  std::string tempNow = root_id, tempLast = "-1";

  while (tempStack.empty() != true)
  {
    tempNow = tempStack.top();
    tempStack.pop();
    nVisisted++;

    std::vector<std::string> vecNeighbor = GetNeighborIDs(tempNow);

    for (auto child = vecNeighbor.begin(); child != vecNeighbor.end(); child++)
    {
      if (!inSquare(*child, square))
        continue;
      if (mapVisited[*child] == false)
      {
        tempStack.push(*child);
        mapVisited[tempNow] = true;
      }
      else
      {
        if (*child != tempLast && tempLast != "-1")
        {
          bFlag = true;
          break;
        }
      }
    }

    if (bFlag)
    {
      break;
    }

    tempLast = tempNow;
  }

  return bFlag;
}

/**
 * FindNearby: Given a class name C, a location name L and a number r,
 * find all locations in class C on the map near L with the range of r and
 * return a vector of string ids
 *
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {int} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName,
                                               std::string name, double r,
                                               int k)
{
  std::vector<std::string> res;
  std::vector<std::pair<std::string, std::string>> vecRes;
  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.attributes.count(attributesName) != 0)
    {
      std::pair<std::string, std::string> temp;
      temp.first = iter->second.name;
      // std::cout <<"!!!!!" << temp.first << std::endl;
      temp.second = iter->second.id;
      vecRes.push_back(temp);
    }
  }
  std::string center_pointer_ID = GetID(name);
  std::vector<std::pair<std::string, double>> temp1;
  for(auto iter = vecRes.begin(); iter != vecRes.end(); iter++){
    // std::string destination_ID = GetID(iter->second);
    double distance_to_center = CalculateDistance(center_pointer_ID, iter->second);
    if(distance_to_center < r && distance_to_center != 0){
      auto pair1 = std::make_pair(iter->first, distance_to_center);  //{name, id}
      temp1.push_back(pair1);
    }
  }
  std::sort(temp1.begin(), temp1.end(), mycmp);
  // for(auto i : temp1){
  for(auto i = temp1.begin(); i < temp1.begin()+k && i != temp1.end(); i++ ){
    res.push_back(GetID((*i).first));
  }
  return res;
}

bool TrojanMap::mycmp(std::pair<std::string, double> a1, std::pair<std::string, double> a2){
  return a1.second < a2.second;
}


/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 *
 */
void TrojanMap::CreateGraphFromCSVFile()
{
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line))
  {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ','))
    {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else
      {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0]))
          n.attributes.insert(word);
        if (isdigit(word[0]))
          n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}

/**
 * StringToLower: Transform a string to lower case
 *
 * @param {std::string} strInput: input string
 * @return {std::string}: output string
 */
std::string TrojanMap::StringToLower(std::string name)
{
  for (auto i{0}; i < name.size(); i++)
  {
    name[i] = tolower(name[i]);
  }
  return name;
}

/**
 * FindLocationName: Check if a location name exists in the data nodes
 *
 * @param {std::string} strInput: input string
 * @return {bool}: return true if exists, else false
 */
bool TrojanMap::FindLocationName(std::string location)
{
  if (location.size() == 0)
  {
    return false;
  }

  for (auto iter = data.begin(); iter != data.end(); iter++)
  {
    if (iter->second.name == location)
    {
      return true;
    }
  }

  return false;
}