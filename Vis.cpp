//
// Created by samarth on 4/5/17.
//
// visualization utilities
// Samarth Brahmbhatt @ Geogia Tech

#include "Vis.h"

using namespace pcl;
using namespace pcl::visualization;
using namespace std;

// append count to a word to get ID for adding an object to the PCLViewer
string Vis::get_next_id(string word) {
  stringstream id;
  id << word << count;
  count++;
  return id.str();
}

Vis::Vis(string window_name) : viewer(new PCLVisualizer(window_name, false)),
                               count(0), shown(false) {
  viewer->setBackgroundColor(0, 0, 0);
  viewer->addCoordinateSystem(0.2);
  viewer->setShowFPS(false);
}

// Visualize point cloud in PCLViewer
bool Vis::addPointCloud(PointCloud<PointXYZ>::ConstPtr cloud, string id,
                        double pt_size) {
  if (id.empty()) id = get_next_id("cloud");

  bool done = false;
  if (!(done = viewer->updatePointCloud<PointXYZ>(cloud, id))) {
    done = viewer->addPointCloud<PointXYZ>(cloud, id);
  }

  if(done) {
    viewer->setPointCloudRenderingProperties(PCL_VISUALIZER_POINT_SIZE, pt_size,
                                             id);
  }
  return done;
}

bool Vis::addPointCloud(PointCloud<PointXYZRGB>::ConstPtr cloud, string id,
                        double pt_size) {
  if (id.empty()) id = get_next_id("cloud");
  PointCloudColorHandlerRGBField<PointXYZRGB> ch(cloud);

  bool done = false;
  if (!(done = viewer->updatePointCloud<PointXYZRGB>(cloud, ch, id))) {
    done = viewer->addPointCloud<PointXYZRGB>(cloud, ch, id);
  }

  if(done) {
    viewer->setPointCloudRenderingProperties(PCL_VISUALIZER_POINT_SIZE, pt_size, id);
  }
  return done;
}

bool Vis::addPointCloud(PointCloud<PointXYZI>::ConstPtr cloud, string id,
                        double pt_size) {
  if (id.empty()) id = get_next_id("cloud");
  PointCloudColorHandlerGenericField<PointXYZI> ch(cloud, "intensity");

  bool done = false;
  if (!(done = viewer->updatePointCloud<PointXYZI>(cloud, ch, id))) {
    done = viewer->addPointCloud<PointXYZI>(cloud, ch, id);
  }

  if (done) {
    viewer->setPointCloudRenderingProperties(PCL_VISUALIZER_POINT_SIZE, pt_size,
                                             id);
  }
  return done;
}

// Visualize point cloud with custom color handler
template <typename PointT>
bool Vis::addPointCloud(typename PointCloud<PointT>::ConstPtr cloud,
                        vector<double> colors, string id, double pt_size) {
  if (id.empty()) id = get_next_id("cloud");
  PointCloudColorHandlerCustom<PointT> ch(cloud, colors[0]*255, colors[1]*255,
      colors[2]*255);

  bool done = false;
  if (!(done = viewer->updatePointCloud<PointT>(cloud, ch, id))) {
    done = viewer->addPointCloud<PointT>(cloud, ch, id);
  }

  if(done) {
    viewer->setPointCloudRenderingProperties(PCL_VISUALIZER_POINT_SIZE, pt_size,
                                             id);
  }
  return done;
}
// instantiation
template bool Vis::addPointCloud<PointXYZ>(PointCloud<PointXYZ>::ConstPtr cloud,
                                           vector<double> colors, string id, double pt_size);
template bool Vis::addPointCloud<PointXYZRGB>(PointCloud<PointXYZRGB>::ConstPtr cloud,
                                              vector<double> colors, string id, double pt_size);
template bool Vis::addPointCloud<PointXYZI>(PointCloud<PointXYZI>::ConstPtr cloud,
                                              vector<double> colors, string id, double pt_size);

template <typename PointT, typename PointNT>
bool Vis::addPointCloudNormals(typename PointCloud<PointT>::ConstPtr cloud,
                               typename PointCloud<PointNT>::ConstPtr normals) {
  string id = get_next_id("normals");
  bool done = viewer->addPointCloudNormals<PointT, PointNT>(cloud, normals, 20, 0.02f, id);
  return done;
}
// instantiation
template bool Vis::addPointCloudNormals<PointXYZRGB, Normal>(PointCloud<PointXYZRGB>::ConstPtr cloud,
                                                             PointCloud<Normal>::ConstPtr normals);

template <typename PointT>
bool Vis::addLine(PointT p0, PointT p1, vector<double> colors) {
  string id = get_next_id("line");
  bool done = viewer->addLine<PointT, PointT>(p0, p1, colors[0], colors[1],
                                              colors[2], id);
  return done;
}
// instantiation
template bool Vis::addLine(PointXYZ p0, PointXYZ p1, vector<double> colors);
template bool Vis::addLine(PointXYZRGB p0, PointXYZRGB p1, vector<double> colors);

bool Vis::addText3D(string text, PointXYZ p, vector<double> text_colors) {
  string id = get_next_id("text");
  bool done = viewer->addText3D(text, p, 1.0/20, text_colors[0], text_colors[1],
                                text_colors[2]);
  return done;
}

// add wireframe cube from 3D BB
bool Vis::addCube(PointXYZ min_pt, PointXYZ max_pt, vector<double> colors) {
  bool done = true;
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, min_pt.y, min_pt.z),
                  PointXYZ(max_pt.x, min_pt.y, min_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, min_pt.y, min_pt.z),
                  PointXYZ(min_pt.x, max_pt.y, min_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(max_pt.x, min_pt.y, min_pt.z),
                  PointXYZ(max_pt.x, max_pt.y, min_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, max_pt.y, min_pt.z),
                  PointXYZ(max_pt.x, max_pt.y, min_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, min_pt.y, min_pt.z),
                  PointXYZ(min_pt.x, min_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, max_pt.y, min_pt.z),
                  PointXYZ(min_pt.x, max_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(max_pt.x, min_pt.y, min_pt.z),
                  PointXYZ(max_pt.x, min_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(max_pt.x, max_pt.y, min_pt.z),
                  PointXYZ(max_pt.x, max_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, min_pt.y, max_pt.z),
                  PointXYZ(max_pt.x, min_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, min_pt.y, max_pt.z),
                  PointXYZ(min_pt.x, max_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(min_pt.x, max_pt.y, max_pt.z),
                  PointXYZ(max_pt.x, max_pt.y, max_pt.z), colors);
  done &= addLine<PointXYZ>(PointXYZ(max_pt.x, min_pt.y, max_pt.z),
                  PointXYZ(max_pt.x, max_pt.y, max_pt.z), colors);
  return done;
}

// add a sphere
template<typename PointT>
bool Vis::addSphere(PointT p, double radius, vector<double> colors) {
  string id = get_next_id("sphere");
  bool done = viewer->addSphere(p, radius, colors[0], colors[1], colors[2], id);
  return done;
}
// instantiation
template bool Vis::addSphere(PointXYZ p, double radius, vector<double> colors);
template bool Vis::addSphere(PointXYZRGB p, double radius, vector<double> colors);

template <typename PointT>
bool Vis::addCorrespondences(typename PointCloud<PointT>::ConstPtr source,
                             typename PointCloud<PointT>::ConstPtr target,
                             CorrespondencesConstPtr const &corrs, int skip) {
  string id = get_next_id("correspondences");
  // bool done = viewer->addCorrespondences<PointT>(source, target, *corrs, id);
  bool done = true;
  for (int i = 0; i < corrs->size(); i+=skip) {
    done &= addLine<PointT>(source->at(corrs->at(i).index_query),
                            target->at(corrs->at(i).index_match));
  }
  return done;
}
// instantiation
template bool Vis::addCorrespondences<PointXYZ>(PointCloud<PointXYZ>::ConstPtr source,
                                                PointCloud<PointXYZ>::ConstPtr target,
                                                CorrespondencesConstPtr const &corrs, int skip);
template bool Vis::addCorrespondences<PointXYZRGB>(PointCloud<PointXYZRGB>::ConstPtr source,
                                                   PointCloud<PointXYZRGB>::ConstPtr target,
                                                   CorrespondencesConstPtr const &corrs, int skip);

template <typename PointT>
bool Vis::addCorrespondences(typename PointCloud<PointT>::ConstPtr source,
                             typename PointCloud<PointT>::ConstPtr target,
                             vector<int> const &corrs, int skip) {
  string id = get_next_id("correspondences");
  //bool done = viewer->addCorrespondences<PointT>(source, target, corrs, id);
  bool done = true;
  for (int i = 0; i < corrs.size(); i+=skip) {
    done &= addLine<PointT>(source->at(i), target->at(corrs[i]));
  }
  return done;
}
// instantiation
template bool Vis::addCorrespondences<PointXYZ>(PointCloud<PointXYZ>::ConstPtr source,
                                                PointCloud<PointXYZ>::ConstPtr target,
                                                vector<int> const &corrs, int skip);
template bool Vis::addCorrespondences<PointXYZRGB>(PointCloud<PointXYZRGB>::ConstPtr source,
                                                   PointCloud<PointXYZRGB>::ConstPtr target,
                                                   vector<int> const &corrs, int skip);

void Vis::show(bool block) {
  if(!shown) {
    viewer->createInteractor();
    shown = true;
  }
  do {
    viewer->spinOnce(100);
    pcl_sleep(0.1);
  } while(block && !viewer->wasStopped());
  viewer->resetStoppedFlag();
  viewer->close();
}

bool Vis::addPlane(const ModelCoefficientsConstPtr &coeffs) {
  string id = get_next_id("plane");
  bool done = viewer->addPlane(*coeffs, id);
  return done;
}
bool Vis::addPlane(const ModelCoefficientsConstPtr &coeffs, double x, double y,
                   double z) {
  string id = get_next_id("plane");
  bool done = viewer->addPlane(*coeffs, x, y, z, id);
  return done;
}
