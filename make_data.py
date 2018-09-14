import pandas as pd
import numpy as np


if __name__ == '__main__':
  n_row, n_col = 100000, 5000
  a = np.random.randint(0, 100, (n_row, n_col))
  
  cols = ['{:06d}'.format(i) for i in range(n_col)]
  a = pd.DataFrame(a, columns=cols)
  a.to_csv('{:d}_rows_{:d}_cols.csv'.format(n_row, n_col), index=None)
