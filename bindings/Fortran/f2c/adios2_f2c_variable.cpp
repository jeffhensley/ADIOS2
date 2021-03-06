/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * adios2_f2c_variable.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include "adios2_f2c_common.h"

#include "adios2/helper/adiosFunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

void FC_GLOBAL(adios2_variable_name_f2c,
               ADIOS2_VARIABLE_NAME_F2C)(char name[4096], int *size,
                                         const adios2_variable **variable,
                                         int *ierr)
{
    *size = -1;
    size_t sizeC;
    *ierr = static_cast<int>(adios2_variable_name(name, &sizeC, *variable));
    if (*ierr == static_cast<int>(adios2_error_none))
    {
        *size = static_cast<int>(sizeC);
    }
}

void FC_GLOBAL(adios2_variable_type_f2c,
               ADIOS2_VARIABLE_TYPE_F2C)(int *type,
                                         const adios2_variable **variable,
                                         int *ierr)
{
    *type = -1;
    adios2_type typeC;
    *ierr = static_cast<int>(adios2_variable_type(&typeC, *variable));
    if (*ierr == static_cast<int>(adios2_error_none))
    {
        *type = static_cast<int>(typeC);
    }
}

void FC_GLOBAL(adios2_variable_ndims_f2c,
               ADIOS2_VARIABLE_NDIMS_F2C)(int *ndims,
                                          const adios2_variable **variable,
                                          int *ierr)
{
    *ndims = -1;
    size_t ndimsC;
    *ierr = static_cast<int>(adios2_variable_ndims(&ndimsC, *variable));
    if (*ierr == static_cast<int>(adios2_error_none))
    {
        *ndims = static_cast<int>(ndimsC);
    }
}

void FC_GLOBAL(adios2_variable_shape_f2c,
               ADIOS2_VARIABLE_SHAPE_F2C)(int64_t *shape,
                                          const adios2_variable **variable,
                                          int *ierr)
{
    size_t ndims;
    *ierr = static_cast<int>(adios2_variable_ndims(&ndims, *variable));
    if (*ierr > 0)
    {
        return;
    }

    size_t shapeC[ndims];
    *ierr = static_cast<int>(adios2_variable_shape(shapeC, *variable));
    if (*ierr > 0)
    {
        return;
    }

    for (auto d = 0; d < ndims; ++d)
    {
        shape[d] = static_cast<int64_t>(shapeC[d]);
    }
}

void FC_GLOBAL(adios2_variable_steps_f2c,
               adios2_variable_STEPS_F2C)(int64_t *steps,
                                          const adios2_variable **variable,
                                          int *ierr)
{
    *steps = -1;
    size_t stepsC;
    *ierr = static_cast<int>(adios2_variable_steps(&stepsC, *variable));
    if (*ierr == static_cast<int>(adios2_error_none))
    {
        *steps = static_cast<int64_t>(stepsC);
    }
}

void FC_GLOBAL(adios2_set_selection_f2c,
               ADIOS2_SET_SELECTION_F2C)(adios2_variable **variable,
                                         const int *ndims, const int64_t *start,
                                         const int64_t *count, int *ierr)
{
    auto lf_IntToSizeT = [](const int64_t *dimensions, const int size,
                            std::vector<std::size_t> &output) {

        output.resize(size);

        for (unsigned int d = 0; d < size; ++d)
        {
            output[d] = dimensions[d];
        }

    };

    try
    {
        if (start == nullptr || count == nullptr || ndims == nullptr)
        {
            throw std::invalid_argument("ERROR: either start_dims, count_dims "
                                        "or ndims is a null pointer, in call "
                                        "to adios2_set_selection\n");
        }
        std::vector<std::size_t> startV, countV;
        lf_IntToSizeT(start, *ndims, startV);
        lf_IntToSizeT(count, *ndims, countV);
        *ierr = static_cast<int>(adios2_set_selection(
            *variable, *ndims, startV.data(), countV.data()));
    }
    catch (...)
    {
        *ierr = static_cast<int>(
            adios2::helper::ExceptionToError("adios2_set_selection"));
    }
}

void FC_GLOBAL(adios2_set_step_selection_f2c,
               ADIOS2_SET_STEP_SELECTION_F2C)(adios2_variable **variable,
                                              const int64_t *step_start,
                                              const int64_t *step_count,
                                              int *ierr)
{
    try
    {
        if (step_start == nullptr || step_count == nullptr)
        {
            throw std::invalid_argument(
                "ERROR: either step_start or step_count "
                "are null pointers, in call to "
                "adios2_set_step_selection\n");
        }

        if (step_start[0] < 0)
        {
            throw std::invalid_argument("ERROR: negative step_start in call to "
                                        "adios2_set_step_selection\n");
        }

        if (step_count[0] < 0)
        {
            throw std::invalid_argument("ERROR: negative step_count in call to "
                                        "adios2_set_step_selection\n");
        }

        const std::size_t stepStart = static_cast<std::size_t>(*step_start);
        const std::size_t stepCount = static_cast<std::size_t>(*step_count);
        *ierr = adios2_set_step_selection(*variable, stepStart, stepCount);
    }
    catch (...)
    {
        *ierr = static_cast<int>(
            adios2::helper::ExceptionToError("adios2_set_selection"));
    }
}

void FC_GLOBAL(adios2_add_operation_f2c,
               ADIOS2_ADD_OPERATION_F2C)(int *operation_id,
                                         adios2_variable **variable,
                                         adios2_operator **op, const char *key,
                                         const char *value, int *ierr)
{
    *operation_id = -1;
    size_t operation_idC;
    *ierr = static_cast<int>(
        adios2_add_operation(&operation_idC, *variable, *op, key, value));
    if (*ierr == static_cast<int>(adios2_error_none))
    {
        *operation_id = static_cast<int>(operation_idC);
    }
}

void FC_GLOBAL(adios2_set_operation_parameter_f2c,
               ADIOS2_SET_OPERATION_PARAMETER_F2C)(adios2_variable **variable,
                                                   const int *operation_id,
                                                   const char *key,
                                                   const char *value, int *ierr)
{
    try
    {
        if (*operation_id < 0)
        {
            throw std::invalid_argument("ERROR: operation_id can't be "
                                        "negative, in call to "
                                        "adios2_set_operation_paramter");
        }

        *ierr = static_cast<int>(adios2_set_operation_parameter(
            *variable, static_cast<std::size_t>(*operation_id), key, value));
    }
    catch (...)
    {
        *ierr = static_cast<int>(
            adios2::helper::ExceptionToError("adios2_set_operation_parameter"));
    }
}

#ifdef __cplusplus
}
#endif
