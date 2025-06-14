FROM python

WORKDIR /app
COPY agent.py .

ENTRYPOINT ["python", "agent.py"]
