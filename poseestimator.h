#ifndef POSEESTIMATOR_H
#define POSEESTIMATOR_H

#include "warp_functions.h"
#include "pcl_includes.h"
#include <pcl/filters/voxel_grid.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/transformation_estimation_lm.h>
#include <boost/shared_ptr.hpp>

class PoseEstimator
{
private:
  typedef Eigen::Matrix4f tformT;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  PoseEstimator(PointCloudT::ConstPtr const &scene = nullptr,
                PointCloudT::ConstPtr const &object = nullptr);

  // setters
  void set_scene_leaf_size(float s)  {scene_leaf_size = s;}
  void set_object_leaf_size(float s) {object_leaf_size = s;}
  void set_scene_boxsize_x(float s)  {scene_boxsize_x = s;}
  void set_scene_boxsize_y(float s)  {scene_boxsize_y = s;}
  void set_scene_boxsize_z(float s)  {scene_boxsize_z = s;}
  void set_scene_distorted(bool s)   {scene_distorted = s;}
  void set_object_init_x(float s)    {object_init_x = s;}
  void set_object_init_y(float s)    {object_init_y = s;}
  void set_object_init_z(float s)    {object_init_z = s;}
  void set_object_init_dx(float s)   {object_init_dx = s;}
  void set_object_init_dy(float s)   {object_init_dy = s;}
  void set_object_init_dz(float s)   {object_init_dz = s;}
  void set_object_init_azim(float s) {object_init_azim = s;}
  void set_icp_outlier_dist(float s) {icp_outlier_rejection_thresh = s;}
  void set_icp_corr_dist(float s)    {icp_max_corr_distance = s;}
  void set_forced_object_scale(float s) {forced_object_scale = s;}
  void set_icp_use_recip_corr(bool s){icp_use_reciprocal_corr = s;}
  void set_icp_no_rollpitch(bool s)  {icp_no_rollpitch = s;}
  void set_icp_symmetric_object(bool s)  {icp_symmetric_object = s;}
  void set_icp_only_xy(bool s)       {icp_only_xy = s;}
  void set_height_adjust(float s)    {height_adjust = s;}
  void set_scale_axis(char a)        {scale_axis = a;}
  void set_scene(PointCloudT::Ptr const &p);
  void set_object(PointCloudT::Ptr const &p);
  void set_tt_pose(const tformT &T);
  void set_object_flip_angles(float rx, float ry, float rz);
  void set_azim_search_range(float s) {azim_search_range = s;}
  void set_azim_search_step(float s)  {azim_search_step = s;}
  void set_object_slide(float tx, float ty, float tz) {
    object_flip(0, 3) = tx / 100.f;
    object_flip(1, 3) = ty / 100.f;
    object_flip(2, 3) = tz / 100.f;
  }

  // getters
  float get_scene_leaf_size()    {return scene_leaf_size;}
  float get_object_leaf_size()   {return object_leaf_size;}
  float get_scene_boxsize_x()    {return scene_boxsize_x;}
  float get_scene_boxsize_y()    {return scene_boxsize_y;}
  float get_scene_boxsize_z()    {return scene_boxsize_z;}
  bool  get_scene_distorted()    {return scene_distorted;}
  pcl::PointXYZ get_scene_box_min_pt();
  pcl::PointXYZ get_scene_box_max_pt();
  float get_object_init_x()      {return object_init_x;}
  float get_object_init_y()      {return object_init_y;}
  float get_object_init_z()      {return object_init_z;}
  float get_object_init_dx()     {return object_init_dx;}
  float get_object_init_dy()     {return object_init_dy;}
  float get_object_init_dz()     {return object_init_dz;}
  float get_object_init_azim()   {return object_init_azim;}
  float get_icp_outlier_dist()   {return icp_outlier_rejection_thresh;}
  float get_icp_corr_dist()      {return icp_max_corr_distance;}
  float get_forced_object_scale(){return forced_object_scale;}
  float get_height_adjust()      {return height_adjust;}
  bool  get_icp_use_recip_corr() {return icp_use_reciprocal_corr;}
  bool  get_icp_no_rollpitch()   {return icp_no_rollpitch;}
  bool  get_icp_symmetric_object()   {return icp_symmetric_object;}
  bool  get_icp_only_xy()        {return icp_only_xy;}
  char  get_scale_axis()         {return scale_axis;}
  float get_azim_search_range()  {return azim_search_range;}
  float get_azim_search_step()   {return azim_search_step;}
  PointCloudT::ConstPtr get_processed_scene() {return scene_processed;}
  PointCloudT::ConstPtr get_processed_object();
  PointCloudT::ConstPtr get_scene() {return scene;}
  PointCloudT::ConstPtr get_cropped_subsampled_scene()
  {return scene_cropped_subsampled;}
  pcl::ModelCoefficientsConstPtr get_scene_plane_coeffs()
  {return scene_plane_coeffs;}
  tformT get_object_pose();

  // other functions
  void process_scene();
  void process_object();
  void init_icp();
  float do_icp();
  float do_auto_icp();
  bool write_pose_file(std::string pose_filename, std::string scale_filename);
  bool write_tt_file(std::string tt_base_filename);
  bool write_segmented_object(std::string filename);
  bool estimate_plane_params(std::string from_file=std::string());
  bool set_T_b_f(std::string filename);

private:
  void crop_subsample_scene();
  // pose util functions
  tformT get_tabletop_rot(Eigen::Vector3f obj_normal = Eigen::Vector3f(0, 0, 1));
  tformT invert_pose(tformT const &in);
  pcl::PointXYZ get_minpt_offset();
  bool remove_scene_distortion();
  bool estimate_perpendicular_plane(const PointCloudT::ConstPtr &in,
    const Eigen::Vector3f &normal_axis,
    pcl::ModelCoefficientsPtr coeffs=nullptr,
    PointCloudT::Ptr plane_cloud=nullptr,
    pcl::PointIndicesPtr plane_idx=nullptr,
    float epsAngle=20*M_PI/180.f,
    float inlier_thresh=0.003);

  float scene_leaf_size, object_leaf_size;
  float scene_boxsize_x, scene_boxsize_y, scene_boxsize_z;
  float object_init_x, object_init_y, object_init_z, object_init_azim,
  object_init_dx, object_init_dy, object_init_dz;
  float icp_outlier_rejection_thresh, icp_max_corr_distance;
  size_t icp_n_iters;
  bool icp_use_reciprocal_corr, icp_no_rollpitch, icp_symmetric_object,
  icp_only_xy;
  bool scene_distorted;
  PointCloudT::ConstPtr scene, object;
  PointCloudT::Ptr scene_cropped_subsampled;
  PointCloudT::Ptr scene_processed, object_processed;
  PointCloudT::Ptr scene_object_segmented;
  pcl::VoxelGrid<PointT> scene_vox, object_vox;
  pcl::ModelCoefficientsPtr scene_plane_coeffs, distorted_object_plane_coeffs;
  PointCloudT::Ptr scene_plane_hull_points;
  tformT object_adj_pos, object_adj_rot, object_scale, object_flip;
  float axis_size;  // size of object along an axis in the scene
  char scale_axis;  // which axis to use for scaling
  float forced_object_scale;
  float height_adjust;  // adjustment to height of object
  Eigen::Vector3f tt_axis;  // turntable axis of rotation
  Eigen::Vector3f object_flip_angles;  // angles for flipping object model
  tformT T_b_f, T_c_b, T_f_o, T_icp, T_b_f_offset;  // needed for pose suggestions
  bool T_b_f_offset_locked;
  float white_thresh;  // color threshold for white object segmentation (set to -ve to deactivate)
  float azim_search_range, azim_search_step;
  // objects for restricting ICP degrees of freedom
  typedef pcl::registration::TransformationEstimationLM<PointT, PointT> TELM;
  TELM::Ptr te_lm;
  typedef pcl::registration::WarpPointNoAzim<PointT, PointT> WarpNoAzim;
  WarpNoAzim::Ptr warp_no_azim;
  typedef pcl::registration::WarpPointNoRotation<PointT, PointT> WarpNoRotation;
  WarpNoRotation::Ptr warp_no_rotation;
  typedef pcl::registration::WarpPointNoRollPitch<PointT, PointT> WarpNoRollPitch;
  WarpNoRollPitch::Ptr warp_no_rollpitch;
  typedef pcl::registration::WarpPointXY<PointT, PointT> WarpXY;
  WarpXY::Ptr warp_xy;
  typedef pcl::registration::WarpPoint6D<PointT, PointT> Warp6D;
  Warp6D::Ptr warp_6d;
};


template <typename PointSource, typename PointTarget, typename Scalar = float>
class IterativeClosestPoint_Exposed :
    public pcl::IterativeClosestPoint<PointSource, PointTarget, Scalar> {
  public:
    pcl::CorrespondencesPtr getCorrespondencesPtr() {
      return this->correspondences_;
    }
};

#endif // POSEESTIMATOR_H
