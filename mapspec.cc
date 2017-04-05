#include "mapspec.hpp"
#include "json.hpp"
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;
using json = nlohmann::json;

MapSpec::MapSpec(const string &fileLocation) : m_Width(80), m_Height(200) {
  fstream specFile(fileLocation);
  json j;
  specFile >> j;

  if (j.find("width") != j.end()) {
    m_Width = j["width"].get<unsigned short>();
  }

  if (j.find("height") != j.end()) {
    m_Height = j["height"].get<unsigned short>();
  }

  if (j.find("seed") != j.end()) {
    if (m_pSeed) {
      delete m_pSeed;
    }
    unsigned int a[j["seed"].size()];
    for (int i = 0; i < j["seed"].size(); i++) {
      a[i] = j["seed"][i].get<unsigned int>();
    }
    m_pSeed = new seed_seq(a, a + j["seed"].size());
  } else {
    m_pSeed = new seed_seq();
  }

  m_WallProbability = j["wallpercent"].get<unsigned short>();
  m_FilterSteps = j["steps"].get<unsigned short>();
  m_Radii =
      vector<vector<unsigned short>>(m_FilterSteps, vector<unsigned short>(0));
  m_MinMax = vector<vector<unsigned short>>(m_FilterSteps);
  for (int i = 0; i < j["steplist"].size(); i++) {
    m_Specifiers.push_back(
        j["steplist"][i]["specifiers"].get<unsigned short>());
    m_Iterations.push_back(
        j["steplist"][i]["iterations"].get<unsigned short>());
    for (int k = 0; k < j["steplist"][i]["radii"].size(); k++) {
      m_Radii[i].push_back(j["steplist"][i]["radii"][k]);
      m_MinMax[i].push_back(j["steplist"][i]["minmax"][k][0]);
      m_MinMax[i].push_back(j["steplist"][i]["minmax"][k][1]);
    }
  }
}
MapSpec::~MapSpec() {
  delete m_pSeed;
  m_pSeed = nullptr;
}
string MapSpec::ToString() const {
  stringstream ss;
  ss << GetWallProbability() << endl;
  ss << GetStepCount() << endl;
  for (int i = 0; i < GetStepCount(); i++) {
    ss << GetSpecifier(i) << endl;
    ss << GetIteration(i) << endl;
    for (int j = 0; j < GetSpecifier(i); j++) {
      int min;
      int max;
      GetMinMax(i, j, min, max);
      ss << min << " " << max << endl;
      ss << GetRadius(i, j) << endl;
    }
  }
  return ss.str();
}
unsigned short MapSpec::GetWidth() const { return m_Width; }
unsigned short MapSpec::GetHeight() const { return m_Height; }
seed_seq *MapSpec::GetSeed() const { return m_pSeed; }
unsigned short MapSpec::GetWallProbability() const { return m_WallProbability; }
unsigned short MapSpec::GetStepCount() const { return m_FilterSteps; }
unsigned short MapSpec::GetSpecifier(const int &step) const {
  unsigned short outValue = -1;
  if (step <= m_FilterSteps) {
    outValue = m_Specifiers[step];
  }
  return outValue;
}
unsigned short MapSpec::GetIteration(const int &step) const {
  unsigned short outValue = -1;
  if (step <= m_FilterSteps) {
    outValue = m_Iterations[step];
  }
  return outValue;
}
unsigned short MapSpec::GetRadius(const int &step, const int &specifier) const {
  unsigned short outValue = -1;
  if (step <= m_FilterSteps && specifier <= m_Specifiers[step]) {
    outValue = m_Radii[step][specifier];
  }
  return outValue;
}
void MapSpec::GetMinMax(const int &step, const int &specifier, int &min,
                        int &max) const {
  min = -1;
  max = -1;
  if (step <= m_FilterSteps && specifier <= m_Specifiers[step] * 2) {
    min = m_MinMax[step][specifier * 2];
    max = m_MinMax[step][specifier * 2 + 1];
  }
}
