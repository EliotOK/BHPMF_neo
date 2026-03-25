# BHPMF (Modernized Version for R 4.0+) 🚀

This is a revived and modernized fork of the original **BHPMF** (Bayesian Hierarchical Probabilistic Matrix Factorization) R package. 

The original method was developed to fill gaps in large, hierarchically structured numerical databases (e.g., plant functional trait databases like TRY or GRooT). It utilizes taxonomic hierarchy (e.g., Family -> Genus -> Species) to provide uncertainty-quantified imputation for sparse trait matrices.

## ✨ What's New in this Version?

The original package was archived due to fatal compatibility issues with modern C++ compilers and R updates. **This fork brings BHPMF back to life for modern R environments (Tested on R 4.4.1):**

* **Modern Compiler Compatibility:** Fixed the `too few arguments` errors caused by modern Fortran (BLAS/LAPACK) requiring hidden character length arguments (e.g., in `dgemm`, `dtrmv`).
* **Memory Safety & Crash Prevention:** The original C++ code caused fatal R Session crashes (Segfaults / Exit code -1073741819) when output directories did not exist. This has been completely rewritten using R's native `Rf_error()` to provide safe, readable error messages without crashing your IDE.
* **Format String Fixes:** Fixed C++ integer formatting warnings (`%f` to `%d`).

## 📦 Installation

You can install this modernized version directly from GitHub using `devtools` or `remotes`:

```R
# install.packages("remotes")
remotes::install_github("EliotOK/BHPMF_neo")
``` 
## 💡 Usage & Important Notes
Detailed instructions are available in the package Vignette, but please keep the following crucial points in mind when preparing your trait data:

No "All-NA" Rows Allowed: The cross-validation function (CalculateCvRmse) and the underlying Gibbs sampler will fail if a species has NA for all traits.

Solution: Prune your matrix before gap-filling. Remove rows that are 100% missing. After imputation, you can graft them back using the predicted mean values of their respective Genus or Family.

Absolute Paths Matter: When running GapFilling(), ensure your mean.gap.filled.output.path and std.gap.filled.output.path point to existing directories (e.g., "C:/output/mean.txt" on Windows). Do not use Linux-style /tmp/ paths on Windows.

Data Alignment: Ensure the rows in your trait.info matrix strictly correspond to the rows in your hierarchy.info data frame.

## 🍎 Legacy Note for Mac Users
The following instructions from the original authors may still be helpful for older Mac setups, though modern R binaries often handle this better now:

If you encounter C compiler issues on macOS, make sure your environment is properly set up for OpenMP:

Install Xcode and command line developer tools.

Install openMP: http://thecoatlessprofessor.com/programming/openmp-in-r-on-os-x/

Manually reinstall the header files (if on macOS Sierra/Mojave): https://donatstudios.com/MojaveMissingHeaderFiles

**Note that ALL CODES COMES FROM VIBE CODING**
***
