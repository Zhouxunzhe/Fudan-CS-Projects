import os
import pandas as pd


class InvestmentClass:
    def __init__(self, symbol):
        self.symbol = symbol
        self.data = None

    def fetch_data(self, start_date, end_date, folder_name, is_save=False):
        file_path = f'../data/{folder_name}/{self.symbol}.csv'
        # 首先检查本地文件是否存在
        if os.path.exists(file_path):
            print(f"Loading data from local file: {file_path}")
            self.data = pd.read_csv(file_path)
        else:
            print(f"Local file not found. Fetching data from akshare for {self.symbol}")
            self.fetch_from_akshare(start_date, end_date)
            if is_save and self.data is not None:
                self.save_data(folder_name)

    def save_data(self, folder_name):
        folder_path = f'../data/{folder_name}'
        os.makedirs(folder_path, exist_ok=True)
        file_path = os.path.join(folder_path, f'{self.symbol}.csv')
        self.data.to_csv(file_path, index=False)
        print(f"Data saved to {file_path}")

    def fetch_from_akshare(self, start_date, end_date):
        # 这个方法需要在子类中被覆盖
        raise NotImplementedError("This method should be overridden by subclasses.")

    def basic_analysis(self):
        # 基本数据分析方法
        print(self.data.describe())

    def calculate_technical_indicators(self):
        # 计算技术指标
        pass
