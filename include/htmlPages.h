#ifndef __HTMLPAGES_H__
#define __HTMLPAGES_H__

#include <Arduino.h>

const char htmlForm[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Orden de Cargue de Materiales</title>
    <style>
        * {
            box-sizing: border-box;
        }

        body {
            font-family: "Segoe UI", Tahoma, sans-serif;
            background-color: #f2f5f7;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: flex-start;
            min-height: 100vh;
        }

        .container {
            width: 95%;
            max-width: 450px;
            background: #fff;
            border-radius: 12px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
            margin: 20px;
            padding: 20px;
        }

        h2 {
            text-align: center;
            background-color: #0078d7;
            color: white;
            margin: -20px -20px 20px -20px;
            padding: 15px;
            border-top-left-radius: 12px;
            border-top-right-radius: 12px;
            font-size: 1.2rem;
        }

        form {
            display: flex;
            flex-direction: column;
            gap: 12px;
        }

        label {
            font-weight: 500;
            color: #333;
            margin-bottom: 3px;
        }

        input,
        select,
        textarea {
            width: 100%;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 6px;
            font-size: 15px;
        }

        textarea {
            resize: vertical;
            min-height: 60px;
        }

        .row {
            display: flex;
            gap: 10px;
            flex-wrap: wrap;
        }

        .row>div {
            flex: 1 1 48%;
        }

        button {
            background-color: #0078d7;
            color: white;
            border: none;
            padding: 12px;
            border-radius: 8px;
            font-size: 16px;
            cursor: pointer;
            transition: background 0.3s, transform 0.1s;
        }

        button:hover:not(:disabled) {
            background-color: #005fa3;
            transform: scale(1.02);
        }

        button:disabled {
            background-color: #b0b0b0;
            cursor: not-allowed;
            opacity: 0.8;
            transform: none;
        }

        pre {
            background: #e8f0fe;
            border-radius: 6px;
            padding: 10px;
            white-space: pre-wrap;
            word-wrap: break-word;
            font-size: 14px;
        }

        .toast {
            position: fixed;
            bottom: 20px;
            left: 50%;
            transform: translateX(-50%);
            background: #0078d7;
            color: #fff;
            padding: 12px 25px;
            border-radius: 10px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
            font-weight: 500;
            animation: fadeInOut 3s ease forwards;
        }

        @keyframes fadeInOut {
            0% {
                opacity: 0;
                bottom: 0;
            }

            10%,
            90% {
                opacity: 1;
                bottom: 20px;
            }

            100% {
                opacity: 0;
                bottom: 0;
            }
        }
    </style>
</head>

<body>
    <div class="container">
        <h2>Orden de Cargue de Materiales</h2>
        <form id="orderForm">
            <div class="row">
                <div>
                    <label>Fecha</label>
                    <input type="date" id="date" required />
                </div>
                <div>
                    <label>Hora</label>
                    <input type="time" id="time" step="1" required />
                </div>
            </div>

            <label>Entidad</label>
            <input type="text" id="entity" value="Constructora Andes S.A." required />

            <label>Tipo de material</label>
            <input type="text" id="material_type" value="Grava" />

            <div class="row">
                <div>
                    <label>Destino del material</label>
                    <input type="text" id="destination" value="Obra Vial Los Robles" required />
                </div>
                <div>
                    <label>Origen del material</label>
                    <input type="text" id="origin" value="Cantera El Progreso" />
                </div>
            </div>

            <div class="row">
                <div>
                    <label>Cantidad (m³)</label>
                    <input type="number" step="0.1" id="quantity_m3" value="12.5" />
                </div>
                <div>
                    <label>Valor</label>
                    <input type="number" id="value" value="350000" />
                </div>
            </div>

            <label>Conductor</label>
            <input type="text" id="driver" value="Carlos Pérez" />

            <label>Placa del vehículo</label>
            <input type="text" id="vehicle_plate" value="ABC123" />

            <label>Proveedor de cargue</label>
            <input type="text" id="loading_provider" value="Transportes del Sur" />

            <label>Referencia de la máquina</label>
            <input type="text" id="machine_reference" value="CAT320D" />

            <label for="operator_name">Nombre del operador</label>
            <input type="text" id="operator_name" name="operator_name" value="Juan Gómez" required />

            <label for="observations">Observaciones</label>
            <textarea id="observations" name="observations" maxlength="96">
Carga verificada en sitio por el operador</textarea>

            <button type="submit">Enviar</button>
        </form>

        <h3>JSON generado:</h3>
        <pre id="jsonOutput">{}</pre>
    </div>

    <script>
        const form = document.getElementById("orderForm");
        const date = document.getElementById("date");
        const time = document.getElementById("time");
        const output = document.getElementById("jsonOutput");
        const submitButton = form.querySelector('button[type="submit"]');

        const now = new Date();
        date.valueAsDate = now;
        time.step = "1";
        time.value = now.toTimeString().split(' ')[0];

        form.addEventListener("submit", async (e) => {
            e.preventDefault();

            submitButton.disabled = true;
            submitButton.textContent = "Enviando...";

            const data = {
                ts: getTimestamp(),
                dt: getValue("date"),
                tm: getValue("time"),
                ent: getValue("entity"),
                mat: getValue("material_type"),
                dest: getValue("destination"),
                orig: getValue("origin"),
                qty: getValue("quantity_m3"),
                val: getValue("value"),
                drv: getValue("driver"),
                pl: getValue("vehicle_plate"),
                lp: getValue("loading_provider"),
                mach: getValue("machine_reference"),
                op: getValue("operator_name"),
                obs: getValue("observations"),
            };

            output.textContent = JSON.stringify(data, null, 2);

            try {
                const res = await fetch("/submit", {
                    method: "POST",
                    headers: { "Content-Type": "application/json" },
                    body: JSON.stringify(data),
                });

                const json = await res.json();

                if (json.status === "ok") {
                    showToast("✅ Datos enviados correctamente");
                    form.reset();
                } else {
                    showToast("❌ Error al enviar los datos");
                }
            } catch (err) {
                showToast("⚠️ Error de conexión");
            } finally {
                setTimeout(() => {
                    submitButton.textContent = "Enviar";
                    submitButton.disabled = false;
                }, 3000);
            }
        });

        function getValue(id) {
            const el = document.getElementById(id);
            let value = el.value || '';
            if (typeof value === 'string') {
                value = value.replace(/,/g, '');
            }
            return value && !isNaN(value) ? Number(value) : value.trim();
        }


        function getTimestamp() {
            const dateStr = date.value;
            const timeStr = time.value;
            if (!dateStr || !timeStr) return Math.floor(Date.now() / 1000);
            return Math.floor(new Date(`${dateStr}T${timeStr}`).getTime() / 1000);
        }

        function showToast(msg) {
            const div = document.createElement("div");
            div.className = "toast";
            div.textContent = msg;
            document.body.appendChild(div);
            setTimeout(() => div.remove(), 3300);
        }
    </script>
</body>

</html>
)rawliteral";

const char html404[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Página no encontrada</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: "Poppins", sans-serif;
        }

        body {
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            background: linear-gradient(135deg, #e9f0ff, #ffffff);
            color: #333;
            text-align: center;
        }

        .container {
            max-width: 400px;
            background: #fff;
            padding: 2.5rem;
            border-radius: 20px;
            box-shadow: 0 8px 25px rgba(0, 0, 0, 0.08);
            animation: float 3s ease-in-out infinite;
        }

        h1 {
            font-size: 5rem;
            color: #3a7afe;
            margin-bottom: 0.5rem;
        }

        h2 {
            font-size: 1.5rem;
            margin-bottom: 1rem;
        }

        p {
            font-size: 1rem;
            color: #666;
            margin-bottom: 1.5rem;
        }

        a {
            display: inline-block;
            padding: 0.75rem 1.5rem;
            background: #3a7afe;
            color: #fff;
            border-radius: 10px;
            text-decoration: none;
            font-weight: 500;
            transition: background 0.3s ease;
        }

        a:hover {
            background: #2562ea;
        }

        @keyframes float {

            0%,
            100% {
                transform: translateY(0px);
            }

            50% {
                transform: translateY(-10px);
            }
        }
    </style>
</head>

<body>
    <div class="container">
        <h1>404</h1>
        <h2>Página no encontrada</h2>
        <p>La página que buscas no existe o fue movida.</p>
        <a href="/">Volver al inicio</a>
    </div>
</body>

</html>
)rawliteral";
#endif