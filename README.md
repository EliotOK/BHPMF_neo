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
