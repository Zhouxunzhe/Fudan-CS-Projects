# python3
# 请先安装 OpenAI SDK：`pip3 install openai`
from openai import OpenAI

client = OpenAI(api_key="", base_url="https://api.deepseek.com")

system_prompt = """You are an expert financial analyst and investment advisor. Your role is to provide detailed analysis and recommendations based on the provided company data. You should consider various factors such as financial performance, market conditions, and industry trends to give informed advice. Ensure that your responses are comprehensive, logical, and backed by data.

When analyzing a company, consider the following:
- Financial Health: Evaluate metrics like revenue, profit margins, debt levels, and cash flow.
- Growth Potential: Assess past growth rates, future projections, and the company's position in the market.
- Competitive Advantage: Identify unique strengths and differentiators that give the company an edge over competitors.
- Market Conditions: Analyze the broader market and economic environment that could impact the company's performance.
- Risks: Highlight any potential risks or challenges the company might face.

For each analysis, provide a clear recommendation on whether to buy, hold, or sell the stock, and support your recommendation with detailed reasoning.

Examples of tasks you might perform include:
- Analyzing quarterly earnings reports and their impact on stock prices.
- Comparing a company with its industry peers.
- Assessing the impact of new product launches or strategic initiatives.
- Providing long-term investment strategies based on macroeconomic trends."""

user_prompt = """Analyze the following data for Company XYZ and provide an investment recommendation. The data includes the company's recent quarterly earnings, financial health metrics, growth potential, competitive advantages, market conditions, and risks. Based on this data, recommend whether to buy, hold, or sell the stock, and explain your reasoning.

**Company XYZ Data:**
- **Recent Quarterly Earnings:**
  - Revenue: $10 billion (up 5% YoY)
  - Net Income: $2 billion (up 10% YoY)
  - EPS: $2.50 (up from $2.00 last quarter)
- **Financial Health:**
  - Debt-to-Equity Ratio: 0.3
  - Current Ratio: 1.8
  - Free Cash Flow: $1.5 billion
- **Growth Potential:**
  - Historical Revenue Growth Rate: 8% per year over the past 5 years
  - Projected Revenue Growth Rate: 7% per year for the next 5 years
  - Market Share: 20% in the industry
- **Competitive Advantage:**
  - Leading patent portfolio in the industry
  - Strong brand recognition
  - Exclusive supplier agreements
- **Market Conditions:**
  - Overall industry growth: 6% per year
  - Economic outlook: Stable with moderate growth
- **Risks:**
  - Potential regulatory changes in key markets
  - Increasing competition from new entrants
  - Fluctuations in raw material prices

Please provide a detailed analysis and recommendation based on the provided data."""

response = client.chat.completions.create(
    model="deepseek-chat",
    messages=[
        {"role": "system", "content": system_prompt},
        {"role": "user", "content": user_prompt},
    ]
)

print(response.choices[0].message.content)
