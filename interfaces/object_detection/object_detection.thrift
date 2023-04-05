/******************************************************************************
 *                                                                            *
 * Copyright (C) 2022 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

service ObjectDetectionInterface {
    list<double> get_poses();
    list<double> get_object_position();
    i16 get_distance();
}
