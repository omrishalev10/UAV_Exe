import matplotlib.pyplot as plt
import re
import configparser


def read_sim_params(ini_file_path):
    # Initialize default number of UAVs to 0
    n_uav = 0
    with open(ini_file_path, 'r') as file:
        for line in file:
            if line.startswith('N_uav'):
                _, n_uav_value = line.split()
                n_uav = int(n_uav_value)
                break  # Stop reading once N_uav is found
    return n_uav


def read_uav_data(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    data = []
    for line in lines:
        match = re.search(r"Time: (\d+\.?\d*), UAV: \d+, X: (-?\d+\.?\d*), Y: (-?\d+\.?\d*)", line)
        if match:
            time = float(match.group(1))
            x = float(match.group(2))
            y = float(match.group(3))
            data.append((time, x, y))

    return data


def plot_uav_paths(base_path, n_uav):
    plt.figure(figsize=(10, 6))

    for i in range(1, n_uav + 1):
        file_path = f"{base_path}/UAV{i}.txt"
        data = read_uav_data(file_path)
        times, xs, ys = zip(*data)
        plt.plot(xs, ys, marker='o', label=f'UAV{i}')

        # Annotate each point with its time
        for time, x, y in data:
            plt.annotate(f'{time}s', (x, y), textcoords="offset points", xytext=(0, 10), ha='center')

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.title('UAV Paths')
    plt.legend()
    plt.grid(True)
    plt.show()


# Example usage
ini_file_path = 'Project1\SimParams.ini'  # Adjust the path as necessary
base_path = 'Project1'  # The directory where UAV files are stored
n_uav = read_sim_params(ini_file_path)
plot_uav_paths(base_path, n_uav)
