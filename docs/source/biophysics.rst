.. set up matlab code highlighting
.. highlight:: matlab

.. set up referencing
.. _biophysics:
Biophysics
======================

What equations are we solving?


``xolotl`` numerically integrates conductance-based models. The membrane acts as a capacitor
with capacitance :math:`C_m` and electric potential across the membrane :math:`V`. For charge
separation :math:`Q`, the conservation of charge is given by

.. math:: C_m V = Q
  :label: conservation of charge

Taking the derivative with respect to time

.. math::
  :label: conservation of current
  C_m \frac{dV}{dt} &= \frac{Q}{dt} \\
  &= - \sum_i I_i


where :math:`I_i` is the :math:`i`th transmembrane current. Currents are described by non-Ohmic
resistors with variable conductance :math:`g = 1/R`. For an electrochemical driving force :math:`V - E_i`,
the :math:`i`th current is

.. math:: I_i = g_i(V) (V - E_i)
  :label: current equation
