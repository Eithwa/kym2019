#include "nodehandle.h"
DetectedObject::DetectedObject():
    dis_max(0),
    dis_min(0),
    ang_max(0),
    ang_min(0),
    x(0),
    y(0),
    angle(0),
    distance(0.0),
    size(0),
    LR("Null")
{
}
void DetectedObject::Reset()
{     
    dis_max=0;     //pix
    dis_min=0;     //pix
    ang_max=0;     //pix
    ang_min=0;     //pix
    size=0;
    string LR="NULL";
}
NodeHandle::NodeHandle():
	CenterXMsg(275),
	CenterYMsg(270),
	InnerMsg(44),
	OuterMsg(235),
	FrontMsg(267),
    FieldMsg(OuterMsg-100),
	Camera_HighMsg(650)
{
	Readyaml();
	AngleLUT();
    color_map = ColorFile();
	save_sub = nh.subscribe("interface/bin_save", 1000, &NodeHandle::SaveButton_setting, this);
    for(int i=0; i<( sizeof(Unscaned_Angle)/sizeof(Unscaned_Angle[0]) ); i++){
        Unscaned_Angle[i]=999;
    }
}
void NodeHandle::AngleLUT()
{
	double ang_PI;
	for (int ang = 0 ; ang <= 360; ang++) {
		ang_PI = ang * PI / 180;
		Angle_sin.push_back(sin(ang_PI));
		Angle_cos.push_back(cos(ang_PI));
	}
}
void NodeHandle::Readyaml()
{
	std::string param = YAML_PATH;
	const char *parampath = param.c_str();
	if (ifstream(parampath)) {
		std::string temp = "rosparam load " + param +" /FIRA/vision";
		const char *load = temp.c_str();
		system(load);
		cout << "Read the yaml file SUCCESSED" << endl;
	}else{
        cout << "Read the yaml file FAILED" << endl;
    }
    get_param();
}
void NodeHandle::get_param()
{
	cout<<"get parameter"<<endl;
    //====================中心參數============================
	nh.getParam("/FIRA/vision/Center/Center_X", CenterXMsg);
	nh.getParam("/FIRA/vision/Center/Center_Y", CenterYMsg);
	nh.getParam("/FIRA/vision/Center/Inner", InnerMsg);
	nh.getParam("/FIRA/vision/Center/Outer", OuterMsg);
	nh.getParam("/FIRA/vision/Center/Front", FrontMsg);
	nh.getParam("/FIRA/vision/Center/Camera_high", Camera_HighMsg);
    //====================色彩參數============================
    nh.getParam("/FIRA/vision/HSV/Ball", HSV_red);
    nh.getParam("/FIRA/vision/HSV/Green", HSV_green);
    //==================掃瞄點參數=========================
    nh.getParam("/FIRA/vision/SCAN/Angle_Near_Gap", Angle_Near_GapMsg);
    nh.getParam("/FIRA/vision/SCAN/Magn_Near_Gap", Magn_Near_GapMsg);
    nh.getParam("/FIRA/vision/SCAN/Magn_Near_Start", Magn_Near_StartMsg);
    nh.getParam("/FIRA/vision/SCAN/Magn_Middle_Start", Magn_Middle_StartMsg);
    nh.getParam("/FIRA/vision/SCAN/Magn_Far_Start", Magn_Far_StartMsg);
    nh.getParam("/FIRA/vision/SCAN/Magn_Far_End", Magn_Far_EndMsg);
    nh.getParam("/FIRA/vision/SCAN/Dont_Search_Angle_1", Dont_Search_Angle_1Msg);
    nh.getParam("/FIRA/vision/SCAN/Dont_Search_Angle_2", Dont_Search_Angle_2Msg);
    nh.getParam("/FIRA/vision/SCAN/Dont_Search_Angle_3", Dont_Search_Angle_3Msg);
    nh.getParam("/FIRA/vision/SCAN/Angle_range_1", Angle_range_1Msg);
    nh.getParam("/FIRA/vision/SCAN/Angle_range_2_3", Angle_range_2_3Msg);
    Set_Unscaned_Angle();
}
//===================前置處理結束=========================
int NodeHandle::Frame_Area(int coordinate, int range)
{ 
	if (coordinate < 0) coordinate = 0;
	else if (coordinate >= range) coordinate = range - 1;
	return coordinate;
}
//right -0~-180
//left   0~180
int NodeHandle::Strategy_Angle(int angle)
{
    if (Angle_Adjustment(angle - FrontMsg) < 180)
    {
        angle = Angle_Adjustment(angle - FrontMsg);
    }
    else
    {
        angle = Angle_Adjustment(angle - FrontMsg) - 360;
    }
    return angle;
}
//==========角度調整 修正大於或小於360的角度===============
int NodeHandle::Angle_Adjustment(int angle)
{
	if (angle < 0) return angle + 360;
	else if (angle >= 360) return angle - 360;
	else return angle;
}
//角度間隔
//middle start 到 far start 之間　Angle near gap的值為1/2
//far start 之外 Angle near gap的值為1/4
int NodeHandle::Angle_Interval(int radius)
{
    int angle_gap = 1;
    if (radius <= Magn_Middle_StartMsg)
    {
        angle_gap = Angle_Near_GapMsg;
    }
    else if (radius > Magn_Middle_StartMsg && radius <= Magn_Far_StartMsg)
    {
        angle_gap = Angle_Near_GapMsg / 2;
    }
    else if (radius > Magn_Far_StartMsg && radius <= Magn_Far_EndMsg)
    {
        angle_gap = Angle_Near_GapMsg / 4;
    }
    if (angle_gap <= 0)
    {
        angle_gap = 1;
    }
    return angle_gap;
}
void NodeHandle::Set_Unscaned_Angle()
{
    vector<int> angle_temp;
    angle_temp.push_back(Dont_Search_Angle_1Msg - Angle_range_1Msg);
    angle_temp.push_back(Dont_Search_Angle_1Msg + Angle_range_1Msg);
    angle_temp.push_back(Dont_Search_Angle_2Msg - Angle_range_2_3Msg);
    angle_temp.push_back(Dont_Search_Angle_2Msg + Angle_range_2_3Msg);
    angle_temp.push_back(Dont_Search_Angle_3Msg - Angle_range_2_3Msg);
    angle_temp.push_back(Dont_Search_Angle_3Msg + Angle_range_2_3Msg);

    int counter=0;
    for(int i=0; i<angle_temp.size(); i+=2){
        if(angle_temp.at(i)<=0||angle_temp.at(i+1)>=360){
            if(counter<=4){
                Unscaned_Angle[counter++]=0;
                Unscaned_Angle[counter++]=Angle_Adjustment(angle_temp.at(i+1));
                Unscaned_Angle[counter++]=Angle_Adjustment(angle_temp.at(i));
                Unscaned_Angle[counter++]=360;
            }
        }else{
            if(counter<=6){
                Unscaned_Angle[counter++]=Angle_Adjustment(angle_temp.at(i));
                Unscaned_Angle[counter++]=Angle_Adjustment(angle_temp.at(i+1));
            }
        }
    }
}
//========================save===========================
void NodeHandle::SaveButton_setting(const vision::bin msg)
{
	//cout<<"Save\n";
	SaveButton = msg.bin;
	get_param();
}
//========================distance========================
double NodeHandle::camera_f(double Omni_pixel)
{
	double m = (Omni_pixel * 0.0099) / 60;    // m = H1/H0 = D1/D0    D0 + D1 = 180
	double D0 = 180 / (1 + m);                // D1 = m   *D0
	double D1 = 180 / (1 + (1 / m));          // D0 = 1/m *D1
	double f = 1 / (1 / D0 + 1 / D1);
	//ROS_INFO("m = %f D0 = %f D1 = %f F = %f",m,D0,D1,f);
	return D1;
}
double NodeHandle::Omni_distance(double pixel_dis)
{
	double Z = -1 * Camera_HighMsg; //Camera_HighMsg=650mm;
	//double c  =  D0/2;
	double c = 83.125;
	double b = c * 0.8722;
	double f = camera_f(OuterMsg * 2 * 0.9784);
	double r = atan2(f, pixel_dis * 0.0099);
	double dis = Z * (pow(b, 2) - pow(c, 2)) * cos(r) / ((pow(b, 2) + pow(c, 2)) * sin(r) - 2 * b * c) * 0.1;
	if (dis < 0 || dis > 999) {dis = 999;}
	//ROS_INFO("%f %f %f %f",Z,c,r,dis);
	return dis;
}
//========================color========================
vector<BYTE> NodeHandle::ColorFile()
{
    string Filename = BIN_PATH;
    const char *Filename_Path = Filename.c_str();
    // open the file:
    streampos fileSize;
    std::ifstream file(Filename_Path, ios::binary);
    // get its size:
    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);
    // read the data:
    vector<BYTE> fileData(fileSize);
    file.read((char *)&fileData[0], fileSize);
    return fileData;
}
