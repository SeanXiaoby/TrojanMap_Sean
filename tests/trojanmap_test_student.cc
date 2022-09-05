#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

// Phase 1
// Test Autocomplete function
TEST(TrojanMapTest, Autocomplete1) {
  TrojanMap m;
  // Test the simple case
  auto names = m.Autocomplete("Chi");
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"}; // groundtruth for "Ch"
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true);
  }
}

// Test the lower case
TEST(TrojanMapTest, Autocomplete2) {
  TrojanMap m;
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"};
  auto names = m.Autocomplete("chi");
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true);
  }
}

// Test the lower and upper case 
TEST(TrojanMapTest, Autocomplete3) {
    TrojanMap m;
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"};
  auto names = m.Autocomplete("cHi"); 
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true);
  }
}

  // Test the upper case 
TEST(TrojanMapTest, Autocomplete4) {
    TrojanMap m;
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"};
  auto names = m.Autocomplete("CHI"); 
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true);
  }

}

// Test FindPosition function
TEST(TrojanMapTest, FindPosition0) {
  TrojanMap m;
  // Test Unknown
  auto position = m.GetPosition("a");
  std::pair<double, double> gt0(-1, -1);
  EXPECT_EQ(position, gt0);
  }

  // Test Target
  TEST(TrojanMapTest, FindPosition1) {
  TrojanMap m;
  auto position = m.GetPosition("Target");
  std::pair<double, double> gt1(34.0257016, -118.2843512); // groundtruth for "Target"
  EXPECT_EQ(position, gt1);
  }

  // Test Chick-fil-A
  TEST(TrojanMapTest, FindPosition2) {
  TrojanMap m;
  auto position = m.GetPosition("Chick-fil-A");
  std::pair<double, double> gt2(34.0167334, -118.2825307); // groundtruth for "Chick-fil-A"
  EXPECT_EQ(position, gt2);
  }


// Test CalculateEditDistance function
TEST(TrojanMapTest, CalculateEditDistance0) {
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("horse", "ros"), 3);
}
TEST(TrojanMapTest, CalculateEditDistance1) {
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("horse", ""), 5);
}
TEST(TrojanMapTest, CalculateEditDistance2) {
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("intention", "execution"), 5);
}

// Test FindClosestName function
TEST(TrojanMapTest, FindClosestName0) {
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Rolphs"), "Ralphs");
}
TEST(TrojanMapTest, FindClosestName1) {
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Targeety"), "Target");
}
TEST(TrojanMapTest, FindClosestName2) {
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Adls"), "Arco");
}


// Get All Categories
// This function can only return one result. No need to test :)

// Get all locations in a category

TEST(GetLocationInCategory, Test1){
  TrojanMap m;
  std::vector<std::pair<double, double>> result = { {34.0243 , -118.2840},
                                                     {34.0272 , -118.2880},
                                                     {34.0256 , -118.2770}};
  result = m.GetAllLocationsFromCategory("bar");
  EXPECT_EQ(m.GetAllLocationsFromCategory("bar"), result);
}

TEST(GetLocationInCategory, Test2){
  TrojanMap m;
  std::pair<double, double> tempPair = {34.0305 , -118.2630};
  std::vector<std::pair<double, double>> result = m.GetAllLocationsFromCategory("shoes");
  EXPECT_EQ(m.GetAllLocationsFromCategory("shoes"), result);
}

TEST(GetLocationInCategory, Test3){
  TrojanMap m;
  std::vector<std::pair<double, double>> result = { {34.0243 , -118.2850},
                                                    {34.034 , -118.2650}};
  result = m.GetAllLocationsFromCategory("bicycle");
  EXPECT_EQ(m.GetAllLocationsFromCategory("bicycle"), result);
}


// Get Locations Using A Regular Expression
TEST(GetLocationWithRegex, Test1){
  TrojanMap m;
  std::vector<std::pair<double, double>> result = { {34.0243 , -118.2850},
                                                    {34.034 , -118.2650}};
  result = m.GetLocationRegex("ral");
  EXPECT_EQ(m.GetLocationRegex("ral"), result);
}

TEST(GetLocationWithRegex, Test2){
  TrojanMap m;
  std::vector<std::pair<double, double>> result = { {34.0243 , -118.2850},
                                                    {34.034 , -118.2650}};
  result = m.GetLocationRegex(":d");
  EXPECT_EQ(m.GetLocationRegex(":d"), result);
}

TEST(GetLocationWithRegex, Test3){
  TrojanMap m;
  std::vector<std::pair<double, double>> result = { {34.0243 , -118.2850},
                                                    {34.034 , -118.2650}};
  result = m.GetLocationRegex("x");
  EXPECT_EQ(m.GetLocationRegex("x"), result);
}

// CalculateShortestPath
TEST(TrojanMapTest, CalculateShortestPath_Dijkstra) {
  TrojanMap m;
  
  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Dijkstra("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040153","4380040152","4380040148","6818427920","6818427919",
      "6818427918","6818427892","6818427898","6818427917","6818427916","7232024780","6813416145",
      "6813416154","6813416153","6813416152","6813416151","6813416155","6808069740","6816193785",
      "6816193786","123152294","4015203136","4015203134","4015203133","21098539","6389467809",
      "4015203132","3195897587","4015203129","4015203127","6352865690","6813379589","6813379483",
      "3402887081","6814958394","3402887080","602606656","4872897515","4399697589","6814958391",
      "123209598","6787673296","122728406","6807762271","4399697304","4399697302","5231967015",
      "1862347583","3233702827","4540763379","6819179753","6820935900","6820935901","6813379556",
      "6820935898","1781230450","1781230449","4015405542","4015405543","1837212104","1837212107",
      "2753199985","6820935907","1837212100","4015372458","6813411588","1837212101","6814916516",
      "6814916515","6820935910","4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Dijkstra("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

// Test CalculateShortestPath_Bellman_Ford function
TEST(TrojanMapTest, CalculateShortestPath_Bellman_Ford) {
  TrojanMap m;
  
  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Bellman_Ford("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040153","4380040152","4380040148","6818427920","6818427919",
      "6818427918","6818427892","6818427898","6818427917","6818427916","7232024780","6813416145",
      "6813416154","6813416153","6813416152","6813416151","6813416155","6808069740","6816193785",
      "6816193786","123152294","4015203136","4015203134","4015203133","21098539","6389467809",
      "4015203132","3195897587","4015203129","4015203127","6352865690","6813379589","6813379483",
      "3402887081","6814958394","3402887080","602606656","4872897515","4399697589","6814958391",
      "123209598","6787673296","122728406","6807762271","4399697304","4399697302","5231967015",
      "1862347583","3233702827","4540763379","6819179753","6820935900","6820935901","6813379556",
      "6820935898","1781230450","1781230449","4015405542","4015405543","1837212104","1837212107",
      "2753199985","6820935907","1837212100","4015372458","6813411588","1837212101","6814916516",
      "6814916515","6820935910","4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

// Cycle detection
TEST(CycleDetection, Test1) {
  TrojanMap m;
  
  // Test case 1
  std::vector<double> square1 = {-118.299, -118.264, 34.032, 34.011};
  auto sub1 = m.GetSubgraph(square1);
  bool result1 = m.CycleDetection(sub1, square1);
  EXPECT_EQ(result1, true);
}

TEST(CycleDetection, Test2) {
  TrojanMap m;

  // Test case 2
  std::vector<double> square2 = {-118.290, -118.289, 34.030, 34.020};
  auto sub2 = m.GetSubgraph(square2);
  bool result2 = m.CycleDetection(sub2, square2);
  EXPECT_EQ(result2, false);
}

TEST(CycleDetection, WrongInput) {
  TrojanMap m;
  
  // Test case 1
  std::vector<double> square1 = {-118.290, -118.291, 34.030, 34.020};
  auto sub1 = m.GetSubgraph(square1);
  bool result1 = m.CycleDetection(sub1, square1);
  EXPECT_EQ(result1, false);
}


//Topological sort

// Test cycle detection function
TEST(TrojanMapTest, TopologicalSort0) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
  std::vector<std::vector<std::string>> dependencies = {{"Ralphs","KFC"}, {"Ralphs","Chick-fil-A"}, {"KFC","Chick-fil-A"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"Ralphs", "KFC","Chick-fil-A"};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort1) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC", "Target"};
  std::vector<std::vector<std::string>> dependencies = {{"Ralphs","KFC"}, {"Ralphs","Chick-fil-A"}, {"KFC","Chick-fil-A"}, {"KFC", "Target"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"Ralphs", "KFC","Target", "Chick-fil-A"};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort2) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC", "Arco"};
  std::vector<std::vector<std::string>> dependencies = {{"Ralphs","KFC"}, {"Ralphs","Chick-fil-A"}, {"KFC","Chick-fil-A"}, {"Ralphs", "Arco"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"Ralphs", "Arco", "KFC","Chick-fil-A"};
  EXPECT_EQ(result, gt);
}


// TSP tests
TEST(TrojanMapTest, TSP1) {
  TrojanMap m;
  
  std::vector<std::string> input{"6819019976","6820935923","122702233","8566227783","8566227656","6816180153","1873055993","7771782316"}; // Input location ids 
  auto result = m.TravelingTrojan_Brute_force(input);
  std::cout << "My path length: "  << result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"6819019976","1873055993","8566227656","122702233","8566227783","6816180153","7771782316","6820935923","6819019976"}; // Expected order
  std::cout << "GT path length: "  << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) 
    flag = true;
  
  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, TSP2) {
  TrojanMap m;
  
  std::vector<std::string> input{"6819019976","6820935923","122702233","8566227783","8566227656","6816180153","1873055993","7771782316"}; // Input location ids 
  auto result = m.TravelingTrojan_Backtracking(input);
  std::cout << "My path length: "  << result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"6819019976","1873055993","8566227656","122702233","8566227783","6816180153","7771782316","6820935923","6819019976"}; // Expected order
  std::cout << "GT path length: "  << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) 
    flag = true;
  
  EXPECT_EQ(flag, true);
}

TEST(TrojanMapTest, TSP3) {
  TrojanMap m;
  
  std::vector<std::string> input{"6819019976","6820935923","122702233","8566227783","8566227656","6816180153","1873055993","7771782316"}; // Input location ids 
  auto result = m.TravelingTrojan_2opt(input);
  std::cout << "My path length: "  << result.first << "miles" << std::endl; // Print the result path lengths
  std::vector<std::string> gt{"6819019976","1873055993","8566227656","122702233","8566227783","6816180153","7771782316","6820935923","6819019976"}; // Expected order
  std::cout << "GT path length: "  << m.CalculatePathLength(gt) << "miles" << std::endl; // Print the gt path lengths
  bool flag = false;
  if (gt == result.second[result.second.size()-1]) // clockwise
    flag = true;
  std::reverse(gt.begin(),gt.end()); // Reverse the expected order because the counterclockwise result is also correct
  if (gt == result.second[result.second.size()-1]) 
    flag = true;
  
  EXPECT_EQ(flag, true);
}




// Test FindNearby points
TEST(TrojanMapTest, FindNearby0) {
  TrojanMap m;
  
  auto result = m.FindNearby("supermarket", "Ralphs", 10, 10);
  std::vector<std::string> ans{"5237417649", "6045067406", "7158034317"};
  EXPECT_EQ(result, ans);
}

TEST(TrojanMapTest, FindNearby1) {
  TrojanMap m;
  
  auto result = m.FindNearby("hospital", "Arco", 10, 10);
  std::vector<std::string> ans{"2556689683"};
  EXPECT_EQ(result, ans);
}

TEST(TrojanMapTest, FindNearby2) {
  TrojanMap m;
  
  auto result = m.FindNearby("museum", "Target", 10, 10);
  std::vector<std::string> ans{"4399693647"};
  EXPECT_EQ(result, ans);
}