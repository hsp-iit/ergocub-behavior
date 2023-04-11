/******************************************************************************
 *                                                                            *
 * Copyright (C) 2022 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

service ActionRecognitionInterface {
    i16 get_action();
    bool is_focused();
    double get_distance();
    list<double> get_face_position();
}
