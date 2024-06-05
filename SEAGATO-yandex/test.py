def divide_bytes(data, chunk_size):
    divided_chunks = [data[i:i + chunk_size] for i in range(0, len(data), chunk_size)]
    return divided_chunks

# Usage
def main():
    data = b'This is a sample bytes object.123'
    chunk_size = 5
    divided_chunks = divide_bytes(data, chunk_size)
    for chunk in divided_chunks:
        print(chunk)
if __name__ == "__main__":
    main()