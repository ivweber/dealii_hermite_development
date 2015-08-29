// ---------------------------------------------------------------------
//
// Copyright (C) 1998 - 2015 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------

#ifndef dealii__fe_update_flags_h
#define dealii__fe_update_flags_h


#include <deal.II/base/config.h>
#include <deal.II/base/table.h>
#include <deal.II/base/derivative_form.h>
#include <deal.II/base/point.h>
#include <deal.II/base/tensor.h>

#include <vector>


DEAL_II_NAMESPACE_OPEN

template <int,int> class FiniteElement;


/*!@addtogroup feaccess */
/*@{*/

/**
 * The enum type given to the constructors of FEValues, FEFaceValues and
 * FESubfaceValues, telling those objects which data will be needed on each
 * mesh cell.
 *
 * Selecting these flags in a restrictive way is crucial for the efficiency of
 * FEValues::reinit(), FEFaceValues::reinit() and FESubfaceValues::reinit().
 * Therefore, only the flags actually needed should be selected. It is the
 * responsibility of the involved Mapping and FiniteElement to add additional
 * flags according to their own requirements. For instance, most finite
 * elements will add #update_covariant_transformation if #update_gradients is
 * selected.  By default, all flags are off, i.e. no reinitialization will be
 * done.
 *
 * You can select more than one flag by concatenation using the bitwise or
 * operator|(UpdateFlags,UpdateFlags).
 *
 * <h3>Generating the actual flags</h3>
 *
 * When given a set of UpdateFlags @p flags, the FEValues object must
 * determine, which values will have to be computed once only for the
 * reference cell and which values will have to be updated for each cell.
 * Here, it is important to note that in many cases, the FiniteElement will
 * require additional updates from the Mapping. To this end, several auxiliary
 * functions have been implemented:
 *
 * FiniteElement::update_once(flags) and FiniteElement::update_each(flags)
 * determine the values required by the FiniteElement once or on each cell.
 * The same functions exist in Mapping.
 *
 * Since the FiniteElement does not know if a value required from Mapping
 * should be computed once or for each cell,
 * FEValuesBase::compute_update_flags() is used to compute the union of all
 * values to be computed ever. It does this by first adding to the flags set
 * by the user all flags (once and each) added by the FiniteElement. This new
 * set of flags is then given to the Mapping and all flags required there are
 * added, again once and each.
 *
 * This union of all flags is given to Mapping::fill_fe_values() and
 * FiniteElement::fill_fe_values, where it is split again into the information
 * generated only once and the information that must be updated on each cell.
 *
 * The flags finally stored in FEValues then are the union of all the flags
 * required by the user, by FiniteElement and by Mapping, for computation once
 * or on each cell. Subsequent calls to the functions @p update_once and @p
 * update_each should just select among these flags, but should not add new
 * flags.
 *
 * The mechanism by which all this is accomplished is also discussed on the
 * page on
 * @ref UpdateFlags.
 */
enum UpdateFlags
{
  //! No update
  update_default = 0,
  //! Shape function values
  /**
   * Compute the values of the shape functions at the quadrature points on the
   * real space cell. For the usual Lagrange elements, these values are equal
   * to the values of the shape functions at the quadrature points on the unit
   * cell, but they are different for more complicated elements, such as
   * FE_RaviartThomas elements.
   */
  update_values = 0x0001,
  //! Shape function gradients
  /**
   * Compute the gradients of the shape functions in coordinates of the real
   * cell.
   */
  update_gradients = 0x0002,
  //! Second derivatives of shape functions
  /**
   * Compute the second derivatives of the shape functions in coordinates of
   * the real cell.
   */
  update_hessians = 0x0004,
  //! Outer normal vector, not normalized
  /**
   * Vector product of tangential vectors, yielding a normal vector with a
   * length corresponding to the surface element; may be more efficient than
   * computing both.
   */
  update_boundary_forms = 0x0008,
  //! Transformed quadrature points
  /**
   * Compute the quadrature points transformed into real cell coordinates.
   */
  update_quadrature_points = 0x0010,
  //! Transformed quadrature weights
  /**
   * Compute the quadrature weights on the real cell, i.e. the weights of the
   * quadrature rule multiplied with the determinant of the Jacobian of the
   * transformation from reference to real cell.
   */
  update_JxW_values = 0x0020,
  //! Normal vectors
  /**
   * Compute the normal vectors, either for a face or for a cell of
   * codimension one. Setting this flag for any other object will raise an
   * error.
   */
  update_normal_vectors = 0x0040,
  /**
   * @deprecated Use #update_normal_vectors instead.
   */
  update_face_normal_vectors = update_normal_vectors,
  /**
   * @deprecated Use #update_normal_vectors instead.
   */
  update_cell_normal_vectors = update_normal_vectors,
  //! Volume element
  /**
   * Compute the Jacobian of the transformation from the reference cell to the
   * real cell.
   */
  update_jacobians = 0x0080,
  //! Gradient of volume element
  /**
   * Compute the derivatives of the Jacobian of the transformation.
   */
  update_jacobian_grads = 0x0100,
  //! Volume element
  /**
   * Compute the inverse Jacobian of the transformation from the reference
   * cell to the real cell.
   */
  update_inverse_jacobians = 0x0200,
  //! Covariant transformation
  /**
   * Compute all values the Mapping needs to perform a contravariant
   * transformation of vectors. For special mappings like MappingCartesian
   * this may be simpler than #update_inverse_jacobians.
   */
  update_covariant_transformation = 0x0400,
  //! Contravariant transformation
  /**
   * Compute all values the Mapping needs to perform a contravariant
   * transformation of vectors. For special mappings like MappingCartesian
   * this may be simpler than #update_jacobians.
   */
  update_contravariant_transformation = 0x0800,
  //! Shape function values of transformation
  /**
   * Compute the shape function values of the transformation defined by the
   * Mapping.
   */
  update_transformation_values = 0x1000,
  //! Shape function gradients of transformation
  /**
   * Compute the shape function gradients of the transformation defined by the
   * Mapping.
   */
  update_transformation_gradients = 0x2000,
  //! Determinant of the Jacobian
  /**
   * Compute the volume element in each quadrature point.
   */
  update_volume_elements = 0x4000,
  /**
   * @deprecated This flag has no effect.
   */
  update_support_points = 0x10000,
  //! Jacobian at generalized support points
  /**
   * Update the Jacobian of the mapping in generalized support points.
   */
  update_support_jacobians = 0x20000,
  //! inverse Jacobian at generalized support points
  /**
   * Update the inverse Jacobian of the mapping in generalized support points.
   */
  update_support_inverse_jacobians = 0x40000,
  /**
   * @deprecated Use #update_quadrature_points instead.
   */
  update_q_points = update_quadrature_points,
  /**
   * @deprecated Use #update_hessians instead.
   */
  update_second_derivatives = update_hessians,
  //! Values needed for Piola transform
  /**
   * Combination of the flags needed for Piola transform of Hdiv elements.
   */
  update_piola = update_volume_elements | update_contravariant_transformation
};


/**
 * Output operator which outputs update flags as a set of or'd text values.
 *
 * @ref UpdateFlags
 */
template <class STREAM>
inline
STREAM &operator << (STREAM &s, UpdateFlags u)
{
  s << " UpdateFlags|";
  if (u & update_values)                       s << "values|";
  if (u & update_gradients)                    s << "gradients|";
  if (u & update_hessians)                     s << "hessians|";
  if (u & update_quadrature_points)            s << "quadrature_points|";
  if (u & update_JxW_values)                   s << "JxW_values|";
  if (u & update_normal_vectors)               s << "normal_vectors|";
  if (u & update_jacobians)                    s << "jacobians|";
  if (u & update_inverse_jacobians)            s << "inverse_jacobians|";
  if (u & update_jacobian_grads)               s << "jacobian_grads|";
  if (u & update_covariant_transformation)     s << "covariant_transformation|";
  if (u & update_contravariant_transformation) s << "contravariant_transformation|";
  if (u & update_transformation_values)        s << "transformation_values|";
  if (u & update_transformation_gradients)     s << "transformation_gradients|";
  if (u & update_support_points)               s << "support_points|";
  if (u & update_support_jacobians)            s << "support_jacobians|";
  if (u & update_support_inverse_jacobians)    s << "support_inverse_jacobians|";

//TODO: check that 'u' really only has the flags set that are handled above
  return s;
}


/**
 * Global operator which returns an object in which all bits are set which are
 * either set in the first or the second argument. This operator exists since
 * if it did not then the result of the bit-or <tt>operator |</tt> would be an
 * integer which would in turn trigger a compiler warning when we tried to
 * assign it to an object of type UpdateFlags.
 *
 * @ref UpdateFlags
 */
inline
UpdateFlags
operator | (UpdateFlags f1, UpdateFlags f2)
{
  return static_cast<UpdateFlags> (
           static_cast<unsigned int> (f1) |
           static_cast<unsigned int> (f2));
}




/**
 * Global operator which sets the bits from the second argument also in the
 * first one.
 *
 * @ref UpdateFlags
 */
inline
UpdateFlags &
operator |= (UpdateFlags &f1, UpdateFlags f2)
{
  f1 = f1 | f2;
  return f1;
}


/**
 * Global operator which returns an object in which all bits are set which are
 * set in the first as well as the second argument. This operator exists since
 * if it did not then the result of the bit-and <tt>operator &</tt> would be
 * an integer which would in turn trigger a compiler warning when we tried to
 * assign it to an object of type UpdateFlags.
 *
 * @ref UpdateFlags
 */
inline
UpdateFlags
operator & (UpdateFlags f1, UpdateFlags f2)
{
  return static_cast<UpdateFlags> (
           static_cast<unsigned int> (f1) &
           static_cast<unsigned int> (f2));
}


/**
 * Global operator which clears all the bits in the first argument if they are
 * not also set in the second argument.
 *
 * @ref UpdateFlags
 */
inline
UpdateFlags &
operator &= (UpdateFlags &f1, UpdateFlags f2)
{
  f1 = f1 & f2;
  return f1;
}



/**
 * This enum definition is used for storing similarities of the current cell
 * to the previously visited cell. This information is used for reusing data
 * when calling the method FEValues::reinit() (like derivatives, which do not
 * change if one cell is just a translation of the previous). Currently, this
 * variable does only recognize a translation and an inverted translation (if
 * dim<spacedim). However, this concept makes it easy to add additional
 * states to be detected in FEValues/FEFaceValues for making use of these
 * similarities as well.
 */
namespace CellSimilarity
{
  enum Similarity
  {
    none,
    translation,
    inverted_translation,
    invalid_next_cell
  };
}


namespace internal
{
  namespace FEValues
  {
    /**
     * A class that stores all of the mapping related data used in
     * dealii::FEValues, dealii::FEFaceValues, and dealii::FESubfaceValues
     * objects. Objects of this kind will be given
     * as <i>output</i> argument when dealii::FEValues::reinit()
     * calls Mapping::fill_fe_values() for a given cell, face, or subface.
     *
     * The data herein will then be provided as <i>input</i> argument in
     * the following call to FiniteElement::fill_fe_values().
     *
     * @ingroup feaccess
     */
    template <int dim, int spacedim=dim>
    class MappingRelatedData
    {
    public:
      /**
       * Initialize all vectors to correct size.
       */
      void initialize (const unsigned int n_quadrature_points,
                       const UpdateFlags  flags);

      /**
       * Compute and return an estimate for the memory consumption (in
       * bytes) of this object.
       */
      std::size_t memory_consumption () const;

      /**
       * Store an array of weights times the Jacobi determinant at the quadrature
       * points. This function is reset each time reinit() is called. The Jacobi
       * determinant is actually the reciprocal value of the Jacobi matrices
       * stored in this class, see the general documentation of this class for
       * more information.
       *
       * However, if this object refers to an FEFaceValues or FESubfaceValues
       * object, then the JxW_values correspond to the Jacobian of the
       * transformation of the face, not the cell, i.e. the dimensionality is that
       * of a surface measure, not of a volume measure. In this case, it is
       * computed from the boundary forms, rather than the Jacobian matrix.
       */
      std::vector<double>       JxW_values;

      /**
       * Array of the Jacobian matrices at the quadrature points.
       */
      std::vector< DerivativeForm<1,dim,spacedim> > jacobians;

      /**
       * Array of the derivatives of the Jacobian matrices at the quadrature
       * points.
       */
      std::vector<DerivativeForm<2,dim,spacedim> >  jacobian_grads;

      /**
       * Array of the inverse Jacobian matrices at the quadrature points.
       */
      std::vector<DerivativeForm<1,spacedim,dim> > inverse_jacobians;

      /**
       * Array of quadrature points. This array is set up upon calling reinit()
       * and contains the quadrature points on the real element, rather than on
       * the reference element.
       */
      std::vector<Point<spacedim> >  quadrature_points;

      /**
       * List of outward normal vectors at the quadrature points.
       */
      std::vector<Tensor<1,spacedim> >  normal_vectors;

      /**
       * List of boundary forms at the quadrature points.
       */
      std::vector<Tensor<1,spacedim> >  boundary_forms;
    };


    /**
     * A class that stores all of the shape function related data used in
     * dealii::FEValues, dealii::FEFaceValues, and dealii::FESubfaceValues
     * objects. Objects of this kind will be given
     * as <i>output</i> argument when dealii::FEValues::reinit()
     * calls FiniteElement::fill_fe_values().
     *
     * @ingroup feaccess
     */
    template <int dim, int spacedim=dim>
    class FiniteElementRelatedData
    {
    public:
      /**
       * Initialize all vectors to correct size.
       */
      void initialize (const unsigned int        n_quadrature_points,
                       const FiniteElement<dim,spacedim> &fe,
                       const UpdateFlags         flags);

      /**
       * Compute and return an estimate for the memory consumption (in
       * bytes) of this object.
       */
      std::size_t memory_consumption () const;

      /**
       * Storage type for shape values. Each row in the matrix denotes the values
       * of a single shape function at the different points, columns are for a
       * single point with the different shape functions.
       *
       * If a shape function has more than one non-zero component (in deal.II
       * diction: it is non-primitive), then we allocate one row per non-zero
       * component, and shift subsequent rows backward.  Lookup of the correct row
       * for a shape function is thus simple in case the entire finite element is
       * primitive (i.e. all shape functions are primitive), since then the shape
       * function number equals the row number. Otherwise, use the
       * #shape_function_to_row_table array to get at the first row that belongs
       * to this particular shape function, and navigate among all the rows for
       * this shape function using the FiniteElement::get_nonzero_components()
       * function which tells us which components are non-zero and thus have a row
       * in the array presently under discussion.
       */
      typedef dealii::Table<2,double> ShapeVector;

      /**
       * Storage type for gradients. The layout of data is the same as for the
       * #ShapeVector data type.
       */
      typedef std::vector<std::vector<Tensor<1,spacedim> > > GradientVector;

      /**
       * Likewise for second order derivatives.
       */
      typedef std::vector<std::vector<Tensor<2,spacedim> > > HessianVector;

      /**
       * Store the values of the shape functions at the quadrature points. See the
       * description of the data type for the layout of the data in this field.
       */
      ShapeVector shape_values;

      /**
       * Store the gradients of the shape functions at the quadrature points. See
       * the description of the data type for the layout of the data in this
       * field.
       */
      GradientVector shape_gradients;

      /**
       * Store the 2nd derivatives of the shape functions at the quadrature
       * points.  See the description of the data type for the layout of the data
       * in this field.
       */
      HessianVector shape_hessians;

      /**
       * When asked for the value (or gradient, or Hessian) of shape function i's
       * c-th vector component, we need to look it up in the #shape_values,
       * #shape_gradients and #shape_hessians arrays.  The question is where in
       * this array does the data for shape function i, component c reside. This
       * is what this table answers.
       *
       * The format of the table is as follows: - It has dofs_per_cell times
       * n_components entries. - The entry that corresponds to shape function i,
       * component c is <code>i * n_components + c</code>. - The value stored at
       * this position indicates the row in #shape_values and the other tables
       * where the corresponding datum is stored for all the quadrature points.
       *
       * In the general, vector-valued context, the number of components is larger
       * than one, but for a given shape function, not all vector components may
       * be nonzero (e.g., if a shape function is primitive, then exactly one
       * vector component is non-zero, while the others are all zero). For such
       * zero components, #shape_values and friends do not have a row.
       * Consequently, for vector components for which shape function i is zero,
       * the entry in the current table is numbers::invalid_unsigned_int.
       *
       * On the other hand, the table is guaranteed to have at least one valid
       * index for each shape function. In particular, for a primitive finite
       * element, each shape function has exactly one nonzero component and so for
       * each i, there is exactly one valid index within the range
       * <code>[i*n_components, (i+1)*n_components)</code>.
       */
      std::vector<unsigned int> shape_function_to_row_table;
    };
  }
}


/*@}*/



DEAL_II_NAMESPACE_CLOSE

#endif
