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

Taking the derivative with respect to time

.. math::
  C_m \frac{dV}{dt} &= \frac{dQ}{dt} \\
  &= - \sum_i I_i


where :math:`I_i` is the :math:`i` th transmembrane current. Currents are described by non-Ohmic
resistors with variable conductance :math:`g = 1/R`. For an electrochemical driving force :math:`V - E_i`,
the :math:`i` th current is

.. math:: I_i = g_i(V) (V - E_i)

These currents are produced by ions passing through transmembrane ion channel proteins in and out
of the cell. Since ion channels can be open, closed, or inactivated, the conductance :math:`g` is represented
by the maximal conductance :math:`\bar{g}` which represents the conductance is all channels are open and deinactivated,
and 'gating variable' activation and inactivation terms :math:`m` and :math:`h` which are bounded on :math:`(m,h)~\in~[0,1]`. In this way, they
produce a proportion. When :math:`m=0` all channels are closed. When :math:`m=1` all channels are open. When :math:`h=0`,
all channels are inactivated. When :math:`h=1`, all channels are deinactivated.

.. math::
  g_i(V) = \bar{g_i} m_i^{p_i} h_i^{q_i}

:math:`p` and :math:`q` are fitting constants that take the form of nonnegative integers. For example, the fast sodium
current in Liu *et al.* 1998 has the form

.. math::
  I_{Na} = \bar{g}_{Na} m_{Na}^3 h_{Na} (V-E_{Na})

The gating variables evolve as functions of voltage and time according to the first-order differential equation

.. math::
  \tau_x (V)\frac{dx}{dt} = x_{\infty}(V) - x

where :math:`x = (m,h)`

``xolotl`` solves this equation using the exponential Euler method.

.. math::
  \tau_x (V)\frac{dx}{dt} &= x_{\infty}(V) - x \\
  \int \frac{dx}{-x_{\infty}(V) + x} &= \int -\frac{dt}{\tau_x (V)} \\
  \ln(-x_{\infty}(V) + x) &= \frac{-t}{\tau_x (V)} \\
  x(t) &= x_{\infty}(V) + \exp\Bigg(\frac{-t}{\tau_x (V)}\Bigg)

Accounting for integration constants and using a discrete time-step :math:`\Delta t`,
the exponential Euler method computes the value of the gating variable at the next time step
:math:`t + \Delta t`.

.. math:: x(t + \Delta t) = x_{\infty}(V(t)) + \Big(x(t) - x_{\infty}(V(t))\Big)\exp\Bigg(-\frac{\Delta t}{\tau_x(V(t))}\Bigg)

The membrane potential is integrated in a similar method. If the differential equation for membrane potential
is represented in the form

.. math::
  \tau_V \frac{dV}{dt} = V_{\infty} - V

then it can be numerically integrated by the exponential euler method.

.. math:: V(t + \Delta t) = V_{\infty}(t) + \Big(V(t) - V_{\infty}(t)\Big)\exp\Bigg(-\frac{\Delta t}{\tau_V(t)}\Bigg)

where

.. math::
  V_\infty(t) &= \frac{\sum_i g_i(t) E_i + I_{ext}/A}{\sum_i g_i(t)} \\
  \tau_V(t) &= \frac{C_m}{\sum_i g_i(t)}

where :math:`I_{ext}` is the injected current and :math:`A` is the surface
area of the membrane.

.. hint::
  See `Dayan & Abbott 2001`__ Chapters 5 and 6 for more information.

__ http://www.gatsby.ucl.ac.uk/~lmate/biblio/dayanabbott.pdf
