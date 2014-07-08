__kernel void VectorAdd(__global const float* a, __global const float* b, __global float* c, int iNumElements)
{
  // get index into global data array
  int iGID = get_global_id(0);

  // bound check (equivalent to the limit on a 'for' loop for standard/serial C code
  if (iGID >= iNumElements)
  { 
    return;
  }
  
  // add the vector elements
  c[iGID] = a[iGID] + b[iGID];
}